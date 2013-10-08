#include "ScreenManager.h"

#include "MenuState.h"

#include <boost/filesystem/fstream.hpp>
#include <iostream>
#include <sstream>

inline float screenSizeToClip( const float _screenDimension, const float _screenSize )
{
    return _screenSize / _screenDimension * 2.f;
}
 
inline float screenPositionToClip( const float _screenDimension, const float _screenPosition )
{
    return _screenPosition / _screenDimension * 2.f - 1.f;
}

ScreenManager::ScreenManager(const boost::filesystem::path& _rootDir, FunctionHandler* _funcHandler)
	: funcHandler(_funcHandler)
{
	readScreen(_rootDir / "resources" / "screens.txt");
	//registerScreenState("login", ScreenState::ptr(new MenuState("login")));
	
}

ScreenManager::~ScreenManager()
{
}

bool readFloats(std::vector<float>& _ret, const std::string& _strVal)
{
	std::vector<float> ret;
	std::istringstream ss(_strVal);

	while (ss && !ss.eof())
	{
		float val;
		if (ss >> val)
		{
			ret.push_back(val);

			if (!ss.eof())
			{
				char sl;
				ss.get(sl);

				if (sl != '/')
				{
					return false;
				}
			}
		}
		else
		{
			return false;
		}
	}

	_ret = std::move(ret);

	return true;
}

bool readButton(Button& _button, std::istream& _is, glm::vec2 _screenSize)
{
	glm::vec3 buttonPosition;
	glm::vec2 buttonSize;
	std::string textureNormal, textureHovered, textName, buttonId, buttonFunc;

	while(true)
	{
		std::string line;
		if (!std::getline(_is, line))
		{
			return false;
		}

		if (line.empty())
		{
			continue;
		}

		if (line == "\t/button")
		{
			break;
		}

		std::istringstream ss(line);
		char t1, t2;
		ss.get(t1);
		ss.get(t2);
		if (t1 != '\t' || t2 != '\t')
		{
			return false;
		}

		std::string type, val1;
		ss >> type >> val1;

		if (type.empty())
		{
			continue;
		}

		if (type == "id")
		{
			if (!buttonId.empty() || val1.empty())
			{
				return false;
			}

			buttonId = val1;
		}
		else if(type == "position")
		{
			std::vector<float> posVals;
			if (!readFloats(posVals, val1) || posVals.size() != 3)
			{
				return false;
			}

			buttonPosition.x = screenPositionToClip(_screenSize.x, posVals[0]);
			buttonPosition.y = screenPositionToClip(_screenSize.y, posVals[1]);
			buttonPosition.z = posVals[2];
		}
		else if(type == "size")
		{
			std::vector<float> sizeVals;
			if (!readFloats(sizeVals, val1) || sizeVals.size() != 2)
			{
				return false;
			}

			buttonSize.x = screenSizeToClip(_screenSize.x, sizeVals[0]);
			buttonSize.y = screenSizeToClip(_screenSize.y, sizeVals[1]);
		}
		else if(type == "textureNormal")
		{
			if (!textureNormal.empty() || val1.empty())
			{
				return false;
			}

			textureNormal = val1;
		}
		else if(type == "textureHovered")
		{
			if (!textureHovered.empty() || val1.empty())
			{
				return false;
			}

			textureHovered = val1;
		}
		else if(type == "func")
		{
			if (!buttonFunc.empty() || val1.empty())
			{
				return false;
			}

			buttonFunc = val1;
		}
		else if(type == "textName")
		{
			if(!textName.empty() || val1.empty())
			{
				return false;
			}

			textName = val1;
		}
		else
		{
			return false;
		}
	}

	if (textureNormal.empty() || buttonId.empty() || buttonFunc.empty())
	{
		return false;
	}

	if (textureHovered.empty())
	{
		textureHovered = textureNormal;
	}

	_button.initialize(textureNormal, textureHovered, textName, buttonSize, buttonPosition, buttonFunc);

	return true;
}

bool readInputField(InputField& _inputField, std::istream& _is, glm::vec2 _screenSize)
{
	std::string inputFieldId;
	glm::vec3 inputFieldPosition;
	glm::vec2 inputFieldSize;
	std::string textureName;

	while(true)
	{
		std::string line;
		if (!std::getline(_is, line))
		{
			return false;
		}

		if (line.empty())
		{
			continue;
		}

		if (line == "\t/inputField")
		{
			break;
		}

		std::istringstream ss(line);
		char t1, t2;
		ss.get(t1);
		ss.get(t2);
		if (t1 != '\t' || t2 != '\t')
		{
			return false;
		}

		std::string type, val1;
		ss >> type >> val1;

		if (type.empty())
		{
			continue;
		}

		if (type == "id")
		{
			if (!inputFieldId.empty() || val1.empty())
			{
				return false;
			}

			inputFieldId = val1;
		}
		else if(type == "position")
		{
			std::vector<float> posVals;
			if (!readFloats(posVals, val1) || posVals.size() != 3)
			{
				return false;
			}

			inputFieldPosition.x = screenPositionToClip(_screenSize.x, posVals[0]);
			inputFieldPosition.y = screenPositionToClip(_screenSize.y, posVals[1]);
			inputFieldPosition.z = posVals[2];
		}
		else if(type == "size")
		{
			std::vector<float> sizeVals;
			if (!readFloats(sizeVals, val1) || sizeVals.size() != 2)
			{
				return false;
			}

			inputFieldSize.x = screenSizeToClip(_screenSize.x, sizeVals[0]);
			inputFieldSize.y = screenSizeToClip(_screenSize.y, sizeVals[1]);
		}
		else if(type == "textureName")
		{
			if (!textureName.empty() || val1.empty())
			{
				return false;
			}

			textureName = val1;
		}
		else
		{
			return false;
		}
	}

	if (textureName.empty() || inputFieldId.empty())
	{
		return false;
	}

	_inputField.initialize(inputFieldId, textureName, inputFieldPosition, inputFieldSize);

	return true;
}
bool readImage(Image& _image, std::istream& _is, glm::vec2 _screenSize)
{
	std::string imageId;
	std::string textureName;
	glm::vec3	imagePosition;
	glm::vec2	imageSize;

	while(true)
	{
		std::string line;
		if (!std::getline(_is, line))
		{
			return false;
		}

		if (line.empty())
		{
			continue;
		}

		if (line == "\t/image")
		{
			break;
		}

		std::istringstream ss(line);
		char t1, t2;
		ss.get(t1);
		ss.get(t2);
		if (t1 != '\t' || t2 != '\t')
		{
			return false;
		}

		std::string type, val1;
		ss >> type >> val1;

		if (type.empty())
		{
			continue;
		}

		if (type == "id")
		{
			if (!imageId.empty() || val1.empty())
			{
				return false;
			}

			imageId = val1;
		}
		else if(type == "position")
		{
			std::vector<float> posVals;
			if (!readFloats(posVals, val1) || posVals.size() != 3)
			{
				return false;
			}

			imagePosition.x = screenPositionToClip(_screenSize.x, posVals[0]);
			imagePosition.y = screenPositionToClip(_screenSize.y, posVals[1]);
			imagePosition.z = posVals[2];
		}
		else if(type == "size")
		{
			std::vector<float> sizeVals;
			if (!readFloats(sizeVals, val1) || sizeVals.size() != 2)
			{
				return false;
			}

			imageSize.x = screenSizeToClip(_screenSize.x, sizeVals[0]);
			imageSize.y = screenSizeToClip(_screenSize.y, sizeVals[1]);
		}
		else if(type == "texture")
		{
			if (!textureName.empty() || val1.empty())
			{
				return false;
			}

			textureName = val1;
		}
		else
		{
			return false;
		}
	}

	if (textureName.empty() || imageId.empty())
	{
		return false;
	}

	_image.initialize(imageId, textureName, imagePosition, imageSize);

	return true;
}
bool ScreenManager::readScreen(const boost::filesystem::path& _screenFile)
{
	boost::filesystem::fstream file(_screenFile);

	if(file == NULL)
		return false;


	while(true)
	{
		std::string line;
		
		if(!std::getline(file, line))
			break;

		if (line.empty())
		{
			continue;
		}

		if(line == "screen")
		{
			std::string screenName;
			std::string backgroundName;
			std::vector<Button> buttons;
			std::vector<Image> images;
			std::vector<InputField> inputFields;
			glm::vec2 screenSize(800, 600);

			while(true)
			{
				std::getline(file, line);
				std::istringstream ss(line);

				char t;
				ss.get(t);

				if (line == "/screen")
				{
					break;
				}

				if (t != '\t')
				{
					return false;
				}

				std::string type;
				std::string val1;

				ss >> type >> val1;

				if (type.empty())
				{
					continue;
				}

				if(type == "id")
				{
					if (!screenName.empty() || val1.empty())
					{
						return false;
					}

					screenName = val1;
				}
				else if(type == "background")
				{
					if (!backgroundName.empty() || val1.empty())
					{
						return false;
					}

					backgroundName = val1;
				}
				else if(type == "button")
				{
					Button b(this);
					if (!readButton(b, file, screenSize))
					{
						return false;
					}
					
					buttons.push_back(b);
				}
				else if( type == "image")
				{
					Image image;
					if(!readImage(image, file, screenSize))
					{
						return false;
					}

					images.push_back(image);
				}
				else if(type == "inputField")
				{
					InputField inputField;
					if(!readInputField(inputField, file, screenSize))
						return false;

					inputFields.push_back(inputField);
				}
				else if (type == "screenSize")
				{
					std::vector<float> sizeVals;
					if (!readFloats(sizeVals, val1) || sizeVals.size() != 2)
					{
						return false;
					}

					screenSize.x = sizeVals[0];
					screenSize.y = sizeVals[1];
				}
			}

			if (screenName.empty() || backgroundName.empty())
			{
				return false;
			}

			MenuState::ptr menuState(new MenuState(screenName));
			menuState->addButtons(buttons);
			menuState->addImages(images);
			menuState->addInputFields(inputFields);
			menuState->setBackground(backgroundName);
			registerScreenState(screenName, menuState);		
		}
	}

	return true;
}

void ScreenManager::update(float _dt, std::shared_ptr<IGraphics> _graphics)
{
	//relevant screens for this frame
	std::vector<ScreenState::ptr> screensToUpdate;

	for(auto screen : screens)
	{
		if(screen->isActive())
			screensToUpdate.push_back(screen);
	}

	int numScreens = screensToUpdate.size();
	int i;
	unsigned int topScreen = numScreens;

	for(i=numScreens; i>0; i--)
	{
		ScreenState::ptr currentScreen = screensToUpdate[i-1];

		//several screens can be active at once
		if(currentScreen->isActive())
		{
			currentScreen->update(_dt);
		}
	}

	for(i=numScreens; i>0; i--)
	{
		ScreenState::ptr currentScreen = screensToUpdate[i-1];
		currentScreen->draw(_graphics);

		if (!currentScreen->isPopup())
		{
			break;
		}
	}
}

bool ScreenManager::onInput(const std::vector<IInput::Event>& _events)
{
	//for (auto it = screens.rbegin(); it !=
	screens.back()->onInput(_events);
	return true;
}

bool ScreenManager::registerScreenState(const std::string& _stateId, ScreenState::ptr _state)
{
	if (registeredStates.count(_stateId) != 0)
	{
		return false;
	}

	registeredStates[_stateId] = _state;

	return true;
}

bool ScreenManager::openScreen(const std::string& _stateId)
{
	if (registeredStates.count(_stateId) == 0)
	{
		return false;
	}

	ScreenState::ptr state = registeredStates.at(_stateId);

	if (!state->isInitialized())
	{
		if (!state->initialize())
		{
			return false;
		}
	}

	screens.push_back(state);
	screens.back()->onEntry();

	return true;
}

std::string ScreenManager::getText(const std::string& _elemId) const
{
	return screens.back()->getText(_elemId);
}

bool ScreenManager::goBack()
{
	if (screens.size() < 2)
	{
		return false;
	}

	screens.back()->onExit();
	screens.pop_back();
	screens.back()->onEntry();

	return true;
}

void ScreenManager::buttonPressed(const std::string& _func)
{
	if (_func.substr(0, 5) == "goto/")
	{
		std::string newScreen = _func.substr(5);
		if (!openScreen(newScreen))
		{
			std::cout << "Failed to open screen \"" << newScreen << "\"" << std::endl;
		}
	}
	else if (_func == "back")
	{
		goBack();
	}
	else
	{
		if (funcHandler)
		{
			funcHandler->onFunction(_func);
		}
	}
}

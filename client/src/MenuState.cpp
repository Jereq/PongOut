#include "MenuState.h"
#include <CoreSystem/ICoreSystem.h>


MenuState::MenuState(const std::string _screenName)
	: ScreenState(_screenName)
{
}

MenuState::~MenuState()
{
}

bool MenuState::initialize(std::shared_ptr<ICoreSystem> _iCoreSystem, Server::ptr _server)
{
	if (!ScreenState::initialize(_iCoreSystem, Server::ptr()))
	{
		return false;
	}

	

	return true;
}

bool MenuState::onEntry()
{
	IInput::Event fakeEvent;
	fakeEvent.type = IInput::Event::Type::MOUSE_MOVE;
	fakeEvent.mouseMoveEvent.posY = 10000;
	fakeEvent.mouseMoveEvent.posX = 10000;

	for(Button& b : buttons)
	{
		b.onInput(fakeEvent);
	}

	active = true;
	ICoreSystem::getInstance().lock()->getSounds()->changeBackgroundMusic(music);

	return true;
}


void MenuState::addButtons(std::vector<Button> _buttons)
{
	buttons.insert(buttons.end(),_buttons.begin(), _buttons.end());
}

void MenuState::addImages(std::vector<Image> _images)
{
	images.insert(images.end(),_images.begin(), _images.end());
}

void MenuState::addInputFields(std::vector<InputField> _inputFields)
{
	inputFields.insert(inputFields.end(), _inputFields.begin(), _inputFields.end());
}

void MenuState::onInput(const std::vector<IInput::Event> _events)
{
	for(IInput::Event e : _events)
	{
		switch(e.type)
		{
		case IInput::Event::Type::MOUSE_MOVE:
		case IInput::Event::Type::MOUSE_BUTTON:
			for(Button& b : buttons)
			{
				b.onInput(e);
			}

		case IInput::Event::Type::CHARACTER:
			for (InputField& i : inputFields)
			{
				i.onInput(e);
			}
			break;
		}
	}
}

void MenuState::setBackground(const std::string _backgroundName)
{
	backgroundName = _backgroundName;
}

void MenuState::setMusic(const std::string _music)
{
	music = _music;
}

std::string MenuState::getText(const std::string& _elemId) const
{
	for (const InputField& inF : inputFields)
	{
		if (inF.getId() == _elemId)
		{
			return inF.getText();
		}
	}

	return "";
}

void MenuState::update(float _dt)
{
	for(Button b : buttons)
		b.update(_dt);
}

void MenuState::draw(std::shared_ptr<IGraphics> _graphics)
{
	for(Button b : buttons)
	{
		b.draw(_graphics);
	}

	for(Image i : images)
	{
		i.draw(_graphics);
	}

	for(InputField i : inputFields)
	{
		i.draw(_graphics);
	}
	_graphics->addRectangle(glm::vec3(0,0,0.9999), glm::vec2(2,2), 0, backgroundName);
}

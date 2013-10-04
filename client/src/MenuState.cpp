#include "MenuState.h"

inline float pTcs( const int pixelDimension, const int pixels )
{
	float ret = (float)pixels/pixelDimension*2 -1;
    return ret;
}
 
inline float pTcd( const int pixelDimension, const int pixels )
{
	float ret = (float)pixels/pixelDimension*2;
    return ret;
}

MenuState::MenuState(const std::string _screenName)
	: ScreenState(_screenName)
{
}

MenuState::~MenuState()
{
}

bool MenuState::initialize()
{
	if (!ScreenState::initialize())
	{
		return false;
	}

	FILE* file;
	file = fopen("../screens.txt","r");
	if(file == NULL)
		return false;


	while(true)
	{
		char line[128];
		int res = fscanf(file,"%s",line);
		if(res == EOF)
			break;

		if(strcmp(line,"screen")==0)
		{
			while(strcmp(line,"/screen")!=0)
			{
							//if not correct statid, keep reading in outer loop
			fscanf(file,"%s",line);
			if(strcmp(line,"id")==0)
			{
				fscanf(file,"%s",line);
				if(screenName != std::string(line))	
					break;
			}
				if(strcmp(line,"background")==0)
				{
					fscanf(file,"%s",line);
					backgroundName = std::string(line);
				}

				//fscanf(file,"%s",line);
				if(strcmp(line,"button")==0)
				{
					glm::vec3 buttonPosition;
					glm::vec2 buttonSize;
					std::string textureName, buttonId, buttonFunc;
					while(strcmp(line,"/button")!=0)
					{
						fscanf(file,"%s",line);
						if(strcmp(line,"id")==0)
						{
							fscanf(file,"%s",line);
							buttonId = std::string(line);
						}
						if(strcmp(line,"position")==0)
						{
							float x,y,z;
							fscanf(file,"%f/%f/%f",&x,&y,&buttonPosition.z);
							buttonPosition.x = pTcs(1280,x);
							buttonPosition.y = pTcd(1024,y);
						}
						if(strcmp(line,"size")==0)
						{
							fscanf(file,"%f/%f",&buttonSize.x,&buttonSize.y);
						}
						if(strcmp(line,"texture")==0)
						{
							fscanf(file,"%s",line);
							textureName = std::string(line);
						}
						if(strcmp(line,"func")==0)
						{
							fscanf(file,"%s",line);
							buttonFunc = std::string(line);
						}
					}
					Button* b = new Button();
					
					b->initialize(textureName, "EMPTY", buttonSize, buttonPosition, buttonFunc);
					buttons.push_back(b);
				}
				//fscanf(file,"%s",line);
			}
		}
	}

	return true;
}

void MenuState::onInput(const std::vector<IInput::Event> _events)
{
	for(IInput::Event e : _events)
	{
		switch(e.type)
		{
		case IInput::Event::Type::MOUSE_MOVE:

			
			break;
		}
	}
}

void MenuState::update(float _dt)
{
	for(Button* b : buttons)
		b->update(_dt);
}

void MenuState::draw(std::shared_ptr<IGraphics> _graphics)
{
	for(Button* b : buttons)
	{
		_graphics->addRectangle(b->getCenter(), b->getSize(), 0.0, b->getTextureName());
	}

	_graphics->addRectangle(glm::vec3(0,0,0), glm::vec2(2,2), 0, backgroundName);
}
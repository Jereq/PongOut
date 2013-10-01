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
	active = true;
}

MenuState::~MenuState()
{
}

bool MenuState::initialize()
{
	float x = pTcs(1280, 128);
	float y = pTcd(1024, 10);

	float sx = pTcs(1280, 64);
	float sy = pTcd(1024, 32);
	for(int i=0; i<5; i++)
	{
		Button* b = new Button();
		b->initialize("blocks/orange_01", "ghello", glm::vec2(sx,sy), glm::vec3(x,y,0));
		buttons.push_back(b);
	}
	
	//
	//buttons.push_back(b);
	//glm::vec3 np = b->getCenter();
	//np.g += b->getSize().g;
	//glm::vec2 ns = b->getSize();
	//b = new Button();
	//
	//b->initialize("blocks/dark_magenta_01", "ghello", ns, np);
	//buttons.push_back(b);

	

	return true;
}

void MenuState::onInput()
{
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
}
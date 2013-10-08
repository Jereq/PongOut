#include "MenuState.h"


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

	

	return true;
}

void MenuState::addButtons(std::vector<Button> _buttons)
{
	buttons.insert(buttons.end(),_buttons.begin(), _buttons.end());
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

void MenuState::setBackground(const std::string _backgroundName)
{
	backgroundName = _backgroundName;
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
		_graphics->addRectangle(b.getCenter(), b.getSize(), 0.0, b.getTextureName());
	}

	_graphics->addRectangle(glm::vec3(0,0,0), glm::vec2(2,2), 0, backgroundName);
}
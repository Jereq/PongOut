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

void MenuState::addImages(std::vector<Image> _images)
{
	images.insert(images.end(),_images.begin(), _images.end());
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

	for(Image i : images)
	{
		i.draw(_graphics);
	}
	_graphics->addRectangle(glm::vec3(0,0,0), glm::vec2(2,2), 0, backgroundName);
}
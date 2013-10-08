#include "Button.h"

Button::Button(GUIActionHandler* _actionHandler)
	: actionHandler(_actionHandler)
{

}

Button::~Button()
{

}

bool Button::initialize(const std::string& _backgroundNormal, const std::string& _backgroundHovered, std::string& _textName, 
						const glm::vec2 _size, const glm::vec3 _center, const std::string _func)
{
	backgroundNormal	= _backgroundNormal;
	backgroundHovered	= _backgroundHovered;
	textName	= _textName;
	size		= _size;
	center		= _center;
	func		= _func;
	marked		= false;
	state		= State::Normal;
	return true;
}

const bool Button::isMarked()
{
	return marked;
}

const glm::vec2 Button::getSize()
{
	return size;
}

const glm::vec3 Button::getCenter()
{
	return center;
}

std::string Button::getFunc() const
{
	return func;
}

void Button::onInput(IInput::Event _event)
{
	switch(_event.type)
	{
	case IInput::Event::Type::MOUSE_MOVE:
		{
			double x = _event.mouseMoveEvent.posX;
			double y = _event.mouseMoveEvent.posY;

			marked = (x >= center.x - size.x / 2 && x <= center.x + size.x / 2 &&
							y >= center.y - size.y / 2 && y <= center.y + size.y / 2 );
		}
		switch(state)
		{
		case State::Normal:
			if(marked)
			{
				state = State::Hovered;
			}
			break;
		case State::Hovered:
		case State::Pressed:
			if(!marked)
			{
				state = State::Normal;
			}
			break;
		}
		break;

	case IInput::Event::Type::MOUSE_BUTTON:
		IInput::MouseButtonEvent::Button buttonPressed =  _event.mouseButtonEvent.button;
		bool isPressed = _event.mouseButtonEvent.pressed;
		if(buttonPressed == IInput::MouseButtonEvent::Button::LEFT)
		{
			switch(state)
			{
			case State::Hovered:
				if(isPressed)
				{
					state = State::Pressed;
				}
				break;
			case State::Pressed:
				if(!isPressed)
				{
					state = State::Hovered;
					if(actionHandler != nullptr)
					{
						actionHandler->buttonPressed(func);
					}
				}
				break;
			}
		}
		break;

	}
}

void Button::update(float _dt)
{
	//switch for interpolation later
}

void Button::draw(IGraphics::ptr _graphics)
{
	switch(state)
	{
	case State::Normal:
	case State::Pressed:
		_graphics->addRectangle(center, size, 0, backgroundNormal);
		break;

	case State::Hovered:
		_graphics->addRectangle(center, size, 0, backgroundHovered);
		break;
	}

	if(!textName.empty())
	{
		_graphics->addRectangle(center + glm::vec3(0,0,-0.00001), size * 0.5f, 0, textName);
	}
}
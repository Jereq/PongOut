#include "Button.h"

Button::Button(GUIActionHandler* _actionHandler)
	: actionHandler(_actionHandler)
{

}

Button::~Button()
{

}

bool Button::initialize(const std::string _textureName, const std::string _textName, 
						const glm::vec2 _size, const glm::vec3 _center, const std::string _func)
{
	textureName = _textureName;
	textName	= _textName;
	size		= _size;
	center		= _center;
	func		= _func;
	marked		= false;
	state		= State::Normal;
	return true;
}

const std::string Button::getTextureName()
{
	return textureName;
}

const std::string Button::getTextName()
{
	return textName;
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
	if(marked)
		size = glm::vec2(0.25,0.25);
	else
		size = glm::vec2(0.125,0.125);
}
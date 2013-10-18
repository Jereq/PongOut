#include "InputField.h"

#include <utf8.h>

InputField::InputField()
	: marked(false), state(State::Normal)
{
}

bool InputField::initialize(const std::string& _id,  std::string& _textureName, glm::vec3 _center, glm::vec2 _size)
{
	if(_id.empty())
		return false;

	id			= _id;
	textureName = _textureName;
	center		= _center;
	size		= _size;

	return true;
}

void InputField::onInput(IInput::Event _event)
{
	switch(_event.type)
	{
	case IInput::Event::Type::CHARACTER:
		if (state == State::Selected)
		{
			if (_event.charEvent.character == (char32_t)'\b')
			{
				if (!text.empty())
				{
					auto newEnd = text.end();
					utf8::prior(newEnd, text.begin());
					text = std::string(text.begin(), newEnd);
				}
			}
			else
			{
				utf8::append(_event.charEvent.character, std::back_inserter(text));
			}
		}
		break;

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
					state = State::Selected;
				}
				break;

			case State::Selected:
				if (!marked)
				{
					state = State::Normal;
				}
				break;
			}
		}
		break;

	}
}

void InputField::draw(IGraphics::ptr _graphics)
{
	_graphics->addRectangle(center, size, 0, textureName);
	_graphics->addText("menu_text_field", center - glm::vec3(size.x * 0.45f, size.y * 0.35f, 0.01f), glm::vec2(size.y) * 0.8f, text);
}

std::string InputField::getId() const
{
	return id;
}

std::string InputField::getText() const
{
	return text;
}

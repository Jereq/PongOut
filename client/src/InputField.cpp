#include "InputField.h"

static std::string naiveUTF32toUTF8(char32_t _character)
{
	char buffer[6];

	if (_character <= 0x7F)
	{
		return std::string(1, (char)_character);
	}
	else if (_character <= 0x07FF)
	{
		buffer[1] = (_character & 0x3F) | 0x80;
		_character >>= 6;
		buffer[0] = _character | 0xC0;

		return std::string(buffer, buffer + 2);
	}
	else if (_character <= 0xFFFF)
	{
		buffer[2] = (_character & 0x3F) | 0x80;
		_character >>= 6;
		buffer[1] = (_character & 0x3F) | 0x80;
		_character >>= 6;
		buffer[0] = _character | 0xE0;

		return std::string(buffer, buffer + 3);
	}
	else if (_character <= 0x1FFFFF)
	{
		buffer[3] = (_character & 0x3F) | 0x80;
		_character >>= 6;
		buffer[2] = (_character & 0x3F) | 0x80;
		_character >>= 6;
		buffer[1] = (_character & 0x3F) | 0x80;
		_character >>= 6;
		buffer[0] = _character | 0xF0;

		return std::string(buffer, buffer + 4);
	}
	else if (_character <= 0x3FFFFFF)
	{
		buffer[4] = (_character & 0x3F) | 0x80;
		_character >>= 6;
		buffer[3] = (_character & 0x3F) | 0x80;
		_character >>= 6;
		buffer[2] = (_character & 0x3F) | 0x80;
		_character >>= 6;
		buffer[1] = (_character & 0x3F) | 0x80;
		_character >>= 6;
		buffer[0] = _character | 0xF8;

		return std::string(buffer, buffer + 5);
	}
	else if (_character <= 0x7FFFFFFF)
	{
		buffer[5] = (_character & 0x3F) | 0x80;
		_character >>= 6;
		buffer[4] = (_character & 0x3F) | 0x80;
		_character >>= 6;
		buffer[3] = (_character & 0x3F) | 0x80;
		_character >>= 6;
		buffer[2] = (_character & 0x3F) | 0x80;
		_character >>= 6;
		buffer[1] = (_character & 0x3F) | 0x80;
		_character >>= 6;
		buffer[0] = _character | 0xFC;

		return std::string(buffer, buffer + 6);
	}

	// Invalid
	return std::string();
}

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
					text = text.substr(0, text.size() - 1);
				}
			}
			else
			{
				text += naiveUTF32toUTF8(_event.charEvent.character);
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
}

std::string InputField::getId() const
{
	return id;
}

std::string InputField::getText() const
{
	return text;
}
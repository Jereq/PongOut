#include "InputComponent.h"
#include "GameObject.h"
#include "Paddle.h"

inline glm::vec3 clipToScreenPosition( const glm::vec2 _screenDimension, const glm::vec3 _screenPosition )
{
	return glm::vec3(_screenPosition.x / _screenDimension.x * 2.f - 1.f, _screenPosition.y / _screenDimension.y * 2.f - 1.f, _screenPosition.z);
}

InputComponent::InputComponent()
	: keyLeft(false),
	keyRight(false)
{

}

InputComponent::~InputComponent()
{

}

bool InputComponent::initialize(InputState* _inputState)
{
	inputState = _inputState;

	return true;
}

void InputComponent::update(GameObject* _gameObject, double _dt)
{
	std::vector<IInput::Event> events =  inputState->getEvents();

	for(IInput::Event e : events)
	{
		switch(e.type)
		{
		case IInput::Event::Type::MOUSE_MOVE:
			{
				double x = e.mouseMoveEvent.posX;
				double y = e.mouseMoveEvent.posY;

				_gameObject->velocity = glm::vec2(x,y);
			}
			break;

		case IInput::Event::Type::MOUSE_BUTTON:
			{int i = 42;}
			break;

		case IInput::Event::Type::KEY:
			{
				bool pressed = e.keyEvent.pressed;

				switch (e.keyEvent.key)
				{
				case IInput::KeyCode::SPACE:
					_gameObject->inPlay = true;
					break;
				}
			break;
			}
		}
	}

}

void InputComponent::moveToTarget(Paddle& _paddle, double _dt)
{
	std::vector<IInput::Event> events =  inputState->getEvents();

	for(IInput::Event e : events)
	{
		switch(e.type)
		{
		case IInput::Event::Type::MOUSE_MOVE:
			{
				double x = e.mouseMoveEvent.posX;
				double y = e.mouseMoveEvent.posY;

				_paddle.currentInput = Paddle::InputType::MOUSE;
				_paddle.targetPos = glm::vec2(x,y);
				//_gameObject->velocity = glm::vec2(x,y);
			}
			break;

		case IInput::Event::Type::MOUSE_BUTTON:
			{int i = 42;}
			break;

		case IInput::Event::Type::KEY:
			{
				bool pressed = e.keyEvent.pressed;

				switch (e.keyEvent.key)
				{
				case IInput::KeyCode::A:
				case IInput::KeyCode::LEFT_ARROW:
					_paddle.currentInput = Paddle::InputType::KEYBOARD;
					keyLeft = pressed;
					break;

				case IInput::KeyCode::D:
				case IInput::KeyCode::RIGHT_ARROW:
					_paddle.currentInput = Paddle::InputType::KEYBOARD;
					keyRight = pressed;
					break;
				case IInput::KeyCode::SPACE:
					_paddle.inPlay = true;
					break;
				}

				_paddle.keyDir = (keyLeft ? -1.f : 0.f) + (keyRight ? 1.f : 0.f);
			}
			break;
		}
	}

}
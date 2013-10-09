#include "InputComponent.h"
#include "GameObject.h"

inline glm::vec3 clipToScreenPosition( const glm::vec2 _screenDimension, const glm::vec3 _screenPosition )
{
	return glm::vec3(_screenPosition.x / _screenDimension.x * 2.f - 1.f, _screenPosition.y / _screenDimension.y * 2.f - 1.f, _screenPosition.z);
}

InputComponent::InputComponent()
{

}

InputComponent::~InputComponent()
{

}

bool InputComponent::initialize(InputState::ptr _inputState)
{
	inputState = _inputState;

	return true;
}

void InputComponent::update(GameObject* _gameObject, double _dt)
{
	std::vector<IInput::Event> events =  inputState->getEvents();
	float acceleration =2.f;

	for(IInput::Event e : events)
	{
		switch(e.type)
		{
		case IInput::Event::Type::MOUSE_MOVE:
			{
				double x = e.mouseMoveEvent.posX;
				double y = e.mouseMoveEvent.posY;
			}
			break;

		case IInput::Event::Type::MOUSE_BUTTON:
			{int i = 42;}
			break;

		case IInput::Event::Type::KEY:
			{int a = 42;}
			break;
		}
	}

}
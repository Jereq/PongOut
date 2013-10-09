#include "Paddle.h"

Paddle::Paddle()
	: GameObject()
{
}

Paddle::~Paddle()
{

}

bool Paddle::initialize(const std::string& _id, glm::vec3 _center, glm::vec2 _size, float _rotation, 
					GraphicsComponent::ptr _graphicsComponent, InputComponent::ptr _inputComponent)
{
	if(!GameObject::initialize(_id, _center, _size, _rotation, _graphicsComponent))
		return false;

	inputComponent = _inputComponent;

	return true;
}

void Paddle::update(double _dt)
{
	inputComponent->update(this, _dt);

	center.x = center.x + velocity.x * _dt;
	graphicsComponent->addSpriteToFrame("UNDEFINED", center, size, rotation);
}
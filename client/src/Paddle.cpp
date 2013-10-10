#include "Paddle.h"

Paddle::Paddle()
	: GameObject()
{
}

Paddle::~Paddle()
{

}

bool Paddle::initialize(const std::string& _id, glm::vec3 _center, glm::vec2 _size, float _rotation, 
						GraphicsComponent::ptr _graphicsComponent, InputComponent::ptr _inputComponent, PhysicsComponent::ptr _physicsComponent)
{
	if(!GameObject::initialize(_id, _center, _size, _rotation, _graphicsComponent))
		return false;

	inputComponent		= _inputComponent;
	physicsComponent	= _physicsComponent;
	return true;
}

void Paddle::update(double _dt)
{
	//inputComponent->update(this, _dt);
	inputComponent->moveToTarget(*this, _dt);

	float speedMult = 2.f;
	float maxSpeed = 2.f;
	float accelaration = 2.f;
	
	float diffX = targetPos.x - center.x;
	velocity.x = diffX * accelaration;

	velocity.x = glm::max(velocity.x, -maxSpeed);
	velocity.x = glm::min(velocity.x, maxSpeed);

	center.x = (float)(center.x + velocity.x * speedMult * _dt);

	physicsComponent->restrictToPlayArea(this, _dt);
	
	graphicsComponent->addSpriteToFrame("player/paddle", center, size, rotation);
}
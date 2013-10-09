#include "Ball.h"

Ball::Ball() : GameObject()
{

}

Ball::~Ball()
{

}

bool Ball::initialize(const std::string& _id, glm::vec3 _center, glm::vec2 _size, float _rotation, 
					GraphicsComponent::ptr _graphicsComponent, PhysicsComponent::ptr _physicsComponent)
{
	if(!GameObject::initialize(_id, _center, _size, _rotation, _graphicsComponent))
	{
		return false;
	}

	physicsComponent = _physicsComponent;
	velocity = glm::vec2(0.5,1);
	acceleration = 1.0f;
	return true;
}

void Ball::update(double _dt)
{
	physicsComponent->bounceOnPlayArea(this, _dt);

	center += glm::vec3(velocity, 0) * acceleration * (float)_dt;
	graphicsComponent->addSpriteToFrame("player/ball", center, size, rotation);
}
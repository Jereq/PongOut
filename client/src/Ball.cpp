#include "Ball.h"

Ball::Ball() : GameObject()
{

}

Ball::~Ball()
{

}

bool Ball::initialize(const int _id, glm::vec3 _center, glm::vec2 _size, glm::vec2 _velocity, float _rotation, 
					GraphicsComponent::ptr _graphicsComponent, PhysicsComponent::ptr _physicsComponent)
{
	if(!GameObject::initialize(_id, _center, _size, _rotation, _graphicsComponent))
	{
		return false;
	}

	physicsComponent= _physicsComponent;
	velocity		= _velocity;
	acceleration	= 1.f;
	radius			= size.x * 0.5f;
	inPlay			= false;
	return true;
}

void Ball::setCenter( glm::vec3 _position )
{
	center = _position;
}

void Ball::setInPlay(bool _inPlayState)
{
	GameObject::setInPlay(_inPlayState);

	if(!inPlay)
		velocity = glm::vec2(0,0);
}

void Ball::update(double _dt)
{	
	/*physicsComponent->bounceOnPlayArea(this, _dt);
	physicsComponent->bounceOnBlock(this, _dt);
	physicsComponent->bounceOnPaddle(this, _dt);*/
	
	center += glm::vec3(velocity * acceleration, 0) * (float)_dt;
	graphicsComponent->addSpriteToFrame("player/ball", center, size, rotation);
}
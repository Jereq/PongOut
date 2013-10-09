#include "Ball.hpp"

Ball::Ball(glm::vec3 _position, float _radius, std::string _textureName)
{
	position			= _position;
	radius				= _radius;
	textureName			= _textureName;

	hitBox				= new HitBox(position, glm::vec2(radius * 1.5, radius * 1.5));
}

Ball::~Ball()
{

}

glm::vec3 Ball::getPos()
{
	return position;
}

void Ball::setPos(glm::vec3 _position)
{
	position = _position;
}

glm::vec2 Ball::getVelocity()
{
	return velocity;
}

void Ball::setVelocity(glm::vec2 _velocity)
{
	velocity = _velocity;
}

float Ball::getRadius()
{
	return radius;
}
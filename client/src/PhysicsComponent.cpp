#include "PhysicsComponent.h"
#include "Ball.h"
#include "Paddle.h"

#include "Map.h"
bool PhysicsComponent::initialize(Map* _map)
{
	map = _map;

	if(map == NULL)
		return false;

	return true;
}

void PhysicsComponent::bounceOnPlayArea(GameObject* _gameObject, double _dt)
{
	//return glm::vec2( _screenSize.x / _screenDimension.x * 2.f, _screenSize.y / _screenDimension.y * 2.f);

	glm::vec2 playArea = map->getSize();
	glm::vec2 clipArea = glm::vec2( playArea.x / 1280. * 2.f, playArea.y / 1024. * 2.f);

	glm::vec2 min = glm::vec2(-clipArea.x / 2,-clipArea.y / 2);
	glm::vec2 max = glm::vec2(clipArea.x / 2,clipArea.y / 2);

	if(_gameObject->center.x - _gameObject->size.x / 2 < min.x)
	{
		_gameObject->center.x = min.x + _gameObject->size.x / 2.0;
		_gameObject->velocity.x *= -1;
	}
	else if (_gameObject->center.x + _gameObject->size.x / 2 > max.x)
	{
		_gameObject->center.x = max.x - _gameObject->size.x / 2.0;
		_gameObject->velocity.x *= -1;
	}
	if(_gameObject->center.y - _gameObject->size.y / 2 < min.y)
	{
		_gameObject->center.y = min.y + _gameObject->size.y / 2.0;
		_gameObject->velocity.y *= -1;
	}
	else if(_gameObject->center.y + _gameObject->size.y / 2 > max.y)
	{
		_gameObject->center.y = max.y - _gameObject->size.y / 2.0;
		_gameObject->velocity.y *= -1;
	}
}

void PhysicsComponent::restrictToPlayArea(GameObject* _gameObject, double _dt)
{
		glm::vec2 playArea = map->getSize();
	glm::vec2 clipArea = glm::vec2( playArea.x / 1280. * 2.f, playArea.y / 1024. * 2.f);

	glm::vec2 min = glm::vec2(-clipArea.x / 2,-clipArea.y / 2);
	glm::vec2 max = glm::vec2(clipArea.x / 2,clipArea.y / 2);

	if(_gameObject->center.x - _gameObject->size.x / 2 < min.x)
	{
		_gameObject->center.x = min.x + _gameObject->size.x / 2.0;

	}
	else if (_gameObject->center.x + _gameObject->size.x / 2 > max.x)
	{
		_gameObject->center.x = max.x - _gameObject->size.x / 2.0;

	}
	if(_gameObject->center.y - _gameObject->size.y / 2 < min.y)
	{
		_gameObject->center.y = min.y + _gameObject->size.y / 2.0;

	}
	else if(_gameObject->center.y + _gameObject->size.y / 2 > max.y)
	{
		_gameObject->center.y = max.y - _gameObject->size.y / 2.0;

	}
}

void PhysicsComponent::bounceOnBlock(Ball* _ball, double _dt)
{
	//check ball position one tick forward
	float x	= (_ball->center.x  + _ball->velocity.x * _dt - _ball->size.x / 2.0);	
	float y	= (_ball->center.y  + _ball->velocity.y * _dt - _ball->size.y / 2.0);
	float ballOriginY = y;
	float ballOriginX = x;
	float radius = _ball->radius;

	for(Block::ptr b : map->blocks)
	{
		if(!b->canCollide)
			continue;

		//rectangle
		float rleftX	= b->center.x - b->size.x / 2.0;
		float rrightX	= b->center.x + b->size.x / 2.0;
		float rTopY		= b->center.y + b->size.y / 2.0;
		float rBottomY	= b->center.y - b->size.y / 2.0;

		if(x < rleftX)
			x = rleftX;
		if(x > rrightX)
			x = rrightX;
		if(y < rTopY)
			y = rTopY;
		if(y > rBottomY)
			y = rBottomY;

		if( (ballOriginX - x) * (ballOriginX - x) + (ballOriginY - y) * (ballOriginY - y) < (radius * radius) )
		{
			glm::vec2 n = glm::vec2(b->center.x - _ball->center.x, b->center.y - _ball->center.y);
			glm::vec2 v = _ball->velocity;
			float dotN = glm::dot(n,n);
			float dotV = glm::dot(v,v);
			glm::vec2 newVelocity = _ball->velocity;
			newVelocity -= (2.0f * (dotV / dotN)) * n;
			newVelocity = glm::normalize(newVelocity);
			_ball->velocity = newVelocity;

			b->canCollide = false;
			
			return;
		}

	}
}

void PhysicsComponent::bounceOnPaddle(Ball* _ball, double _dt)
{
	//check ball position one tick forward
	float x	= (_ball->center.x  + _ball->velocity.x * _dt - _ball->size.x / 2.0);	
	float y	= (_ball->center.y  + _ball->velocity.y * _dt - _ball->size.y / 2.0);
	float ballOriginY = y;
	float ballOriginX = x;
	float radius = _ball->radius;

	for(Paddle::ptr paddle : map->paddles)
	{
		//rectangle
		float rleftX	= paddle->center.x - paddle->size.x / 2.0;
		float rrightX	= paddle->center.x + paddle->size.x / 2.0;
		float rTopY		= paddle->center.y + paddle->size.y / 2.0;
		float rBottomY	= paddle->center.y - paddle->size.y / 2.0;

		if(x < rleftX)
			x = rleftX;
		if(x > rrightX)
			x = rrightX;
		if(y < rTopY)
			y = rTopY;
		if(y > rBottomY)
			y = rBottomY;

		if( (ballOriginX - x) * (ballOriginX - x) + (ballOriginY - y) * (ballOriginY - y) < (radius * radius) )
		{
			glm::vec2 n = glm::vec2(paddle->center.x - _ball->center.x, paddle->center.y - _ball->center.y);
			glm::vec2 v = _ball->velocity;
			float dotN = glm::dot(n,n);
			float dotV = glm::dot(v,v);
			glm::vec2 newVelocity = _ball->velocity;
			newVelocity -= (2.0f * (dotV / dotN)) * n;
			newVelocity = glm::normalize(newVelocity);
			_ball->velocity = newVelocity;

		
			
			return;
		}

	}
}
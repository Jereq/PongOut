#include "PhysicsComponent.h"
#include "Ball.h"


bool PhysicsComponent::initialize(Map* _map)
{
	map = _map;

	if(map == NULL)
		return false;

	return true;
}

void PhysicsComponent::update(Ball* _ball, double _dt)
{
	//return glm::vec2( _screenSize.x / _screenDimension.x * 2.f, _screenSize.y / _screenDimension.y * 2.f);

	glm::vec2 playArea = map->getSize();
	glm::vec2 clipArea = glm::vec2( playArea.x / 1280. * 2.f, playArea.y / 1024. * 2.f);

	glm::vec2 min = glm::vec2(-clipArea.x / 2,-clipArea.y / 2);
	glm::vec2 max = glm::vec2(clipArea.x / 2,clipArea.y / 2);

	if(_ball->center.x - _ball->size.x / 2 < min.x)
	{
		_ball->center.x = min.x + _ball->size.x / 2.0;
		_ball->velocity.x *= -1;
	}
	else if (_ball->center.x + _ball->size.x / 2 > max.x)
	{
		_ball->center.x = max.x - _ball->size.x / 2.0;
		_ball->velocity.x *= -1;
	}
	if(_ball->center.y - _ball->size.y / 2 < min.y)
	{
		_ball->center.y = min.y + _ball->size.y / 2.0;
		_ball->velocity.y *= -1;
	}
	else if(_ball->center.y + _ball->size.y / 2 > max.y)
	{
		_ball->center.y = max.y - _ball->size.y / 2.0;
		_ball->velocity.y *= -1;
	}
}
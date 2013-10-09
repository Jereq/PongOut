#include "PhysicsComponent.h"


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
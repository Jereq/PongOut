#include "PhysicsComponent.h"
#include "Ball.h"
#include "Paddle.h"
#include "Map.h"
#include <CoreSystem/ICoreSystem.h>

bool PhysicsComponent::initialize(Map* _map)
{
	map = _map;

	if(map == NULL)
		return false;

	return true;
}

bool PhysicsComponent::circleCirleIntersect(glm::vec2 _center1, float _radius1, glm::vec2 _center2, float _radius2)
{
	glm::vec2 relPos = _center1 - _center2;
	float radiusSum = _radius1 + _radius2;
	return glm::dot(relPos, relPos) <= radiusSum * radiusSum;
}

bool PhysicsComponent::circleLineIntersect(glm::vec2 _circleCenter, float _circleRadius, glm::vec2 _lineStart, glm::vec2 _lineEnd)
{
	glm::vec2 circleRelPos = _circleCenter - _lineStart;
	glm::vec2 lineRelEnd = _lineEnd - _lineStart;

	if (lineRelEnd == glm::vec2(0.f, 0.f))
	{
		return false;
	}

	glm::vec2 lineDir = glm::normalize(lineRelEnd);
	float circleLengthAlong = glm::dot(circleRelPos, lineDir);

	if (circleLengthAlong < 0.f || circleLengthAlong * circleLengthAlong > glm::dot(lineRelEnd, lineRelEnd))
	{
		return false;
	}

	glm::vec2 normDistance = circleRelPos - lineDir * circleLengthAlong;
	float normLengthSqr = glm::dot(normDistance, normDistance);
	
	if (normLengthSqr > _circleRadius * _circleRadius)
	{
		return false;
	}

	return true;
}

bool PhysicsComponent::ballRectCollide(glm::vec2& _reflectDir, Ball* _ball, GameObject* _rect)
{
	glm::vec2 center(_ball->center);

	float rleftX	= _rect->center.x - _rect->size.x * 0.5f;
	float rrightX	= _rect->center.x + _rect->size.x * 0.5f;
	float rTopY		= _rect->center.y + _rect->size.y * 0.5f;
	float rBottomY	= _rect->center.y - _rect->size.y * 0.5f;

	glm::vec2 corners[4] = {
		glm::vec2(rleftX, rTopY),
		glm::vec2(rleftX, rBottomY),
		glm::vec2(rrightX, rBottomY),
		glm::vec2(rrightX, rTopY)
	};

	if (circleLineIntersect(center, _ball->radius, corners[0], corners[1]))
	{
		if (_ball->velocity.x > 0.f)
		{
			_reflectDir = glm::vec2(-1.f, 0.f);
			return true;
		}
	}
	else if (circleLineIntersect(center, _ball->radius, corners[1], corners[2]))
	{
		if (_ball->velocity.y > 0.f)
		{
			_reflectDir = glm::vec2(0.f, -1.f);
			return true;
		}
	}
	else if (circleLineIntersect(center, _ball->radius, corners[2], corners[3]))
	{
		if (_ball->velocity.x < 0.f)
		{
			_reflectDir = glm::vec2(1.f, 0.f);
			return true;
		}
	}
	else if (circleLineIntersect(center, _ball->radius, corners[3], corners[0]))
	{
		if (_ball->velocity.y < 0.f)
		{
			_reflectDir = glm::vec2(0.f, 1.f);
			return true;
		}
	}
	else
	{
		for (glm::vec2& corner : corners)
		{
			if (circleCirleIntersect(center, _ball->radius, corner, 0.f))
			{
				if (glm::dot(_ball->velocity, corner - center) >= 0.f)
				{
					_reflectDir = glm::normalize(center - corner);
					return true;
				}
			}
		}
	}

	return false;
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
		_gameObject->center.x = min.x + _gameObject->size.x / 2.0f;
		_gameObject->velocity.x *= -1;
		ICoreSystem::getInstance().lock()->getSounds()->playSfx("wall_collision");
	}
	else if (_gameObject->center.x + _gameObject->size.x / 2 > max.x)
	{
		_gameObject->center.x = max.x - _gameObject->size.x / 2.0f;
		_gameObject->velocity.x *= -1;
		ICoreSystem::getInstance().lock()->getSounds()->playSfx("wall_collision");
	}
	if(_gameObject->center.y - _gameObject->size.y / 2 < min.y)
	{
		_gameObject->center.y = min.y + _gameObject->size.y / 2.0f;
		_gameObject->velocity.y *= -1;
		ICoreSystem::getInstance().lock()->getSounds()->playSfx("wall_collision");
	}
	else if(_gameObject->center.y + _gameObject->size.y / 2 > max.y)
	{
		_gameObject->center.y = max.y - _gameObject->size.y / 2.0f;
		_gameObject->velocity.y *= -1;
		ICoreSystem::getInstance().lock()->getSounds()->playSfx("wall_collision");
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
		_gameObject->center.x = min.x + _gameObject->size.x / 2.0f;
		_gameObject->velocity = glm::vec2(0.f);

	}
	else if (_gameObject->center.x + _gameObject->size.x / 2 > max.x)
	{
		_gameObject->center.x = max.x - _gameObject->size.x / 2.0f;
		_gameObject->velocity = glm::vec2(0.f);

	}
	if(_gameObject->center.y - _gameObject->size.y / 2 < min.y)
	{
		_gameObject->center.y = min.y + _gameObject->size.y / 2.0f;
		_gameObject->velocity = glm::vec2(0.f);

	}
	else if(_gameObject->center.y + _gameObject->size.y / 2 > max.y)
	{
		_gameObject->center.y = max.y - _gameObject->size.y / 2.0f;
		_gameObject->velocity = glm::vec2(0.f);

	}
}

void PhysicsComponent::bounceOnBlock(Ball* _ball, double _dt)
{
	glm::vec2 center(_ball->center.swizzle(glm::X, glm::Y));

	for (const BlockC::ptr& b : map->blocks)
	{
		if (!b->canCollide)
			continue;

		glm::vec2 reflectDir;

		if (ballRectCollide(reflectDir, _ball, b.get()))
		{
			_ball->velocity = glm::reflect(_ball->velocity, reflectDir);
			
			b->health--;

			if(b->health <= 0)
			{
				b->canCollide = false;
			}
			ICoreSystem::getInstance().lock()->getSounds()->playSfx("block_explodes");
		}
	}
}

void PhysicsComponent::bounceOnPaddle(Ball* _ball, double _dt)
{
	for(Paddle::ptr paddle : map->paddles)
	{
		glm::vec2 reflectDir;
		if (ballRectCollide(reflectDir, _ball, paddle.get()))
		{
			_ball->velocity = glm::normalize(glm::vec2(_ball->center - paddle->center)) * glm::length(_ball->velocity);
			
			ICoreSystem::getInstance().lock()->getSounds()->playSfx("wall_collision");
		}
	}
}
#ifndef __PHYSICSCOMPONENT_H
#define __PHYSICSCOMPONENT_H

#include "boost/filesystem.hpp"
#include <glm/glm.hpp>
class Map;

class GameObject;
class Ball;
class Paddle;


class PhysicsComponent
{
public:
	typedef std::shared_ptr<PhysicsComponent> ptr;
	virtual void bounceOnPlayArea(GameObject* _gameObject, double _dt);
	virtual void bounceOnBlock(Ball* _ball, double _dt);
	virtual void bounceOnPaddle(Ball* _ball, double _dt);
	virtual void restrictToPlayArea(GameObject* _gameObject, double _dt);
	virtual bool initialize(Map* _map);

	static bool circleCirleIntersect(glm::vec2 _center1, float _radius1, glm::vec2 _center2, float _radius2);
	static bool circleLineIntersect(glm::vec2 _circleCenter, float _circleRadius, glm::vec2 _lineStart, glm::vec2 _lineEnd);
	static bool ballRectCollide(glm::vec2& _reflectDir, Ball* _ball, GameObject* _rect);

private:
	Map* map;
	void bindInPlayState(GameObject* _gameObject);
};

#endif
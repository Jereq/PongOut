#ifndef __PHYSICSCOMPONENT_H
#define __PHYSICSCOMPONENT_H

#include "boost/filesystem.hpp"

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

private:
	Map* map;
};

#endif
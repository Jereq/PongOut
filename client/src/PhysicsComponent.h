#ifndef __PHYSICSCOMPONENT_H
#define __PHYSICSCOMPONENT_H

#include "boost/filesystem.hpp"
#include "Block.h"
#include "Paddle.h"
#include "Map.h"

class Ball;

class PhysicsComponent
{
public:
	typedef std::shared_ptr<PhysicsComponent> ptr;
	virtual void update(Ball* _ball, double _dt);
	virtual bool initialize(Map* _map);

private:
	Map* map;
};

#endif
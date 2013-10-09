#ifndef __PHYSICSCOMPONENT_H
#define __PHYSICSCOMPONENT_H

#include "boost/filesystem.hpp"

#include "Map.h"

class GameObject;

class PhysicsComponent
{
public:
	typedef std::shared_ptr<PhysicsComponent> ptr;
	virtual void bounceOnPlayArea(GameObject* _gameObject, double _dt);
	virtual void restrictToPlayArea(GameObject* _gameObject, double _dt);
	virtual bool initialize(Map* _map);

private:
	Map* map;
};

#endif
#ifndef __BALL_H
#define __BALL_H

#include "GameObject.h"
#include "PhysicsComponent.h"

class Ball : public GameObject
{
public:
	Ball();
	~Ball();

	bool initialize(const std::string& _id, glm::vec3 _center, glm::vec2 _size, float _rotation, 
					GraphicsComponent::ptr _graphicsComponent, PhysicsComponent::ptr _physicsComponent);

	typedef std::shared_ptr<Ball> ptr;
private:
	float radius;
	float acceleration;
	PhysicsComponent::ptr physicsComponent;

	virtual void update(double _dt);
	friend class PhysicsComponent;
};

#endif
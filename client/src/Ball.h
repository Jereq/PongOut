#ifndef __BALL_H
#define __BALL_H

#include "GameObject.h"
#include "PhysicsComponent.h"

class Ball : public GameObject
{
public:
	Ball();
	~Ball();

	bool initialize(const int _id, glm::vec3 _center, glm::vec2 _size, glm::vec2 _velocity, float _rotation, 
					GraphicsComponent::ptr _graphicsComponent, PhysicsComponent::ptr _physicsComponent);
	
	virtual void update(double _dt);
	typedef std::shared_ptr<Ball> ptr;
	void setCenter(glm::vec3 _position);
	void setInPlay(bool _inPlayState)override;
private:
	float radius;
	float acceleration;
	PhysicsComponent::ptr physicsComponent;

	
	friend class PhysicsComponent;
};

#endif
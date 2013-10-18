#ifndef __PADDLE_H
#define __PADDLE_H

#include "GameObject.h"
#include "InputComponent.h"
#include "PhysicsComponent.h"

class Paddle : public GameObject
{
private:
	InputComponent::ptr		inputComponent;
	PhysicsComponent::ptr	physicsComponent;
	glm::vec2	targetPos;
	float keyDir;
	enum class InputType
	{
		MOUSE,
		KEYBOARD
	};
	InputType currentInput;
	

	friend class InputComponent;
public:
	Paddle();
	~Paddle();

	bool initialize(const int _id, glm::vec3 _center, glm::vec2 _size, float _rotation, 
		GraphicsComponent::ptr _graphicsComponent, InputComponent::ptr _inputComponent, PhysicsComponent::ptr _physicsComponent);

	virtual void	update(double _dt);

	void setInPlay(bool _inPlayState)override;

	typedef std::shared_ptr<Paddle> ptr;
};

#endif
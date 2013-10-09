#ifndef __PADDLE_H
#define __PADDLE_H

#include "GameObject.h"
#include "InputComponent.h"

class Paddle : public GameObject
{
private:
	InputComponent::ptr inputComponent;

public:
	Paddle();
	~Paddle();

	bool initialize(const std::string& _id, glm::vec3 _center, glm::vec2 _size, float _rotation, 
					GraphicsComponent::ptr _graphicsComponent, InputComponent::ptr _inputComponent);
	virtual void	update(double _dt);
};

#endif
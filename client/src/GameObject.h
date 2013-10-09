#ifndef __GAMEOBJECT_H
#define __GAMEOBJECT_H

#include <boost/filesystem.hpp>
#include <string>

#include "GraphicsComponent.h"

class GameObject
{
public://private:
	std::string		objectId;
	friend class InputComponent;
	friend class PhysicsComponent;
protected:
	GraphicsComponent::ptr graphicsComponent;

	glm::vec3	center;
	glm::vec2	size;
	glm::vec2	velocity;
	float		rotation;

	GameObject(const std::string _objectId = "UNDEFINED");
	virtual ~GameObject();
	bool initialize(const std::string& _id, glm::vec3 _center, glm::vec2 _size, float _rotation, GraphicsComponent::ptr _graphicsComponent);

public:
	typedef std::shared_ptr<GameObject> ptr;

	std::string		getId()const;
	
	virtual void	update(double _dt);
};

#endif
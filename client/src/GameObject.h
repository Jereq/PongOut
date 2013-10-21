#ifndef __GAMEOBJECT_H
#define __GAMEOBJECT_H

#include <boost/filesystem.hpp>
#include <string>

#include "GraphicsComponent.h"

class GameObject
{
private:
	int				objectId;
	friend class	InputComponent;
	friend class	PhysicsComponent;
protected:
	GraphicsComponent::ptr graphicsComponent;

	glm::vec3	center;
	glm::vec2	size;
	glm::vec2	velocity;
	float		rotation;
	bool		inPlay;

	GameObject(const int _objectId = -1);
	virtual ~GameObject();
	bool initialize(const int _id, glm::vec3 _center, glm::vec2 _size, float _rotation, GraphicsComponent::ptr _graphicsComponent);

public:
	typedef std::shared_ptr<GameObject> ptr;

	int				getId()const;
	glm::vec3		getCenter()const;
	glm::vec2		getVelocity()const;
	bool			getInPlay()const;

	virtual void	update(double _dt);
	void			setPosition(glm::vec3 _position, glm::vec2 _velocity);
	virtual void	setInPlay(bool _inPlayState);
};

#endif
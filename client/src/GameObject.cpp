#include "GameObject.h"

inline glm::vec2 screenSizeToClip( const glm::vec2 _screenDimension, const glm::vec2 _screenSize )
{
	return glm::vec2( _screenSize.x / _screenDimension.x * 2.f, _screenSize.y / _screenDimension.y * 2.f);
}
 
inline glm::vec3 screenPositionToClip( const glm::vec2 _screenDimension, const glm::vec3 _screenPosition )
{
	return glm::vec3(_screenPosition.x / _screenDimension.x * 2.f - 1.f, _screenPosition.y / _screenDimension.y * 2.f - 1.f, _screenPosition.z);
}

GameObject::GameObject(const int _objectId)
	: objectId(_objectId)
{

}

GameObject::~GameObject()
{

}

int GameObject::getId()const
{
	return objectId;
}


glm::vec3 GameObject::getCenter()const
{
	return center;
}

bool GameObject::getInPlay()const
{
	return inPlay;
}

void GameObject::setPosition(glm::vec3 _position, glm::vec2 _velocity)
{
	center	= _position;
	velocity= _velocity;
}

glm::vec2 GameObject::getVelocity()const
{
	return velocity;
}

bool GameObject::initialize(const int _id, glm::vec3 _center, glm::vec2 _size, float _rotation, GraphicsComponent::ptr _graphicsComponent)
{
	objectId			= _id;
	center				= _center;
	size				= _size;
	rotation			= _rotation;
	graphicsComponent	= _graphicsComponent;
	velocity			= glm::vec2(0,0);
	inPlay				= false;

	return true;
}

void GameObject::setInPlay(bool _inPlayState)
{
	inPlay = _inPlayState;
}

void GameObject::update(double _dt)
{

	graphicsComponent->addSpriteToFrame("UNDEFINED", center, size, rotation);
}
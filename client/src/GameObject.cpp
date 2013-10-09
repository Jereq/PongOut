#include "GameObject.h"

inline glm::vec2 screenSizeToClip( const glm::vec2 _screenDimension, const glm::vec2 _screenSize )
{
	return glm::vec2( _screenSize.x / _screenDimension.x * 2.f, _screenSize.y / _screenDimension.y * 2.f);
}
 
inline glm::vec3 screenPositionToClip( const glm::vec2 _screenDimension, const glm::vec3 _screenPosition )
{
	return glm::vec3(_screenPosition.x / _screenDimension.x * 2.f - 1.f, _screenPosition.y / _screenDimension.y * 2.f - 1.f, _screenPosition.z);
}

GameObject::GameObject(const std::string _objectId)
{

}

GameObject::~GameObject()
{

}

std::string GameObject::getId()const
{
	return objectId;
}

bool GameObject::initialize(const std::string& _id, glm::vec3 _center, glm::vec2 _size, float _rotation, GraphicsComponent::ptr _graphicsComponent)
{
	objectId			= _id;
	center				= screenPositionToClip(glm::vec2(1280,1024), _center);
	size				= screenSizeToClip(glm::vec2(1280,1024), _size);
	rotation			= _rotation;
	graphicsComponent	= _graphicsComponent;
	velocity			= glm::vec2(0,0);
	return true;
}

void GameObject::update(double _dt)
{

	graphicsComponent->addSpriteToFrame("UNDEFINED", center, size, rotation);
}
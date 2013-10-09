#include "GraphicsComponent.h"
#include "GameObject.h"

inline glm::vec2 screenSizeToClip( const glm::vec2 _screenDimension, const glm::vec2 _screenSize )
{
	return glm::vec2( _screenSize.x / _screenDimension.x * 2.f, _screenSize.y / _screenDimension.y * 2.f);
}
 
inline glm::vec3 screenPositionToClip( const glm::vec2 _screenDimension, const glm::vec3 _screenPosition )
{
	return glm::vec3(_screenPosition.x / _screenDimension.x * 2.f - 1.f, _screenPosition.y / _screenDimension.y * 2.f - 1.f, _screenPosition.z);
}

bool GraphicsComponent::initialize(std::shared_ptr<IGraphics> _graphics)
{
	graphics = _graphics;

	return true;
}

void GraphicsComponent::addSpriteToFrame(const std::string& _textureId, glm::vec3 _center, glm::vec2 _size, float _rotation)
{
	//glm::vec3 center = screenPositionToClip(glm::vec2(1280,1024), _center);
	//glm::vec2 size = screenSizeToClip(glm::vec2(1280,1024), _size);

	graphics->addRectangle(_center, _size, _rotation, _textureId);
}
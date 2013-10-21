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

const static glm::vec2 MAP_SIZE(1200.f, 600.f);
const static float BORDER_COOF = 0.9f;

inline glm::vec2 gameSizeToClip( const glm::vec2 _gameSize )
{
	return glm::vec2( _gameSize.x / MAP_SIZE.x * 2.f, _gameSize.y / MAP_SIZE.y * 2.f) * BORDER_COOF;
}

inline glm::vec3 gamePositionToClip(  const glm::vec3 _gamePosition )
{
	return glm::vec3((_gamePosition.x / MAP_SIZE.x * 2.f - 1.f) * BORDER_COOF, (_gamePosition.y / MAP_SIZE.y * 2.f - 1.f) * BORDER_COOF, _gamePosition.z);
}

bool GraphicsComponent::initialize(std::shared_ptr<IGraphics> _graphics)
{
	graphics = _graphics;

	return true;
}

void GraphicsComponent::addSpriteToFrame(const std::string& _textureId, glm::vec3 _center, glm::vec2 _size, float _rotation)
{
	glm::vec3 center = gamePositionToClip(_center);
	glm::vec2 size = gameSizeToClip(_size);

	graphics->addRectangle(center, size, _rotation, _textureId);
}
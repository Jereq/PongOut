/*
 * Block.cpp
 *
 *
 *
 */

#include "Block.h"

Block::Block() : GameObject()
{
	canCollide = true;
}

Block::~Block() {}

bool Block::initialize(const std::string& _id, glm::vec3 _center, glm::vec2 _size, float _rotation, 
					GraphicsComponent::ptr _graphicsComponent, const std::string& _texture)
{
	if(!GameObject::initialize(_id, _center, _size, _rotation, _graphicsComponent))
		return false;

	texture = _texture;

	return true;
}

void Block::update(double _dt)
{
	if(canCollide)
		graphicsComponent->addSpriteToFrame(texture, center, size, rotation);
}
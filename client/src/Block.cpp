/*
 * Block.cpp
 *
 *
 *
 */

#include "Block.h"

Block::Block()
{
	canCollide = true;
}

Block::~Block() {}

bool Block::initialize(const std::string& _id, glm::vec3 _center, glm::vec2 _size, float _rotation, 
					GraphicsComponent::ptr _graphicsComponent)
{
	if(!GameObject::initialize(_id, _center, _size, _rotation, _graphicsComponent))
		return false;

	return true;
}

void Block::update(double _dt)
{


	//center.x = center.x + velocity.x * _dt;
	if(canCollide)
		graphicsComponent->addSpriteToFrame("blocks/orange_01", center, size, rotation);
}
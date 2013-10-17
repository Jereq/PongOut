/*
 * Block.cpp
 *
 *
 *
 */

#include "BlockC.h"

BlockC::BlockC() : GameObject()
{
	canCollide = true;
}

BlockC::~BlockC() {}

bool BlockC::initialize(const CommonTypes::Block& _blockData, float _rotation, GraphicsComponent::ptr _graphicsComponent)
{
	glm::vec3 center(_blockData.x, _blockData.y, _blockData.z);

	if(!GameObject::initialize("block", center, BLOCKSIZE, _rotation, _graphicsComponent))
		return false;

	textures = _blockData.textures;
	health = _blockData.health;

	return true;
}

void BlockC::update(double _dt)
{
	if(canCollide)
		graphicsComponent->addSpriteToFrame(textures[health-1], center, size, rotation);
}
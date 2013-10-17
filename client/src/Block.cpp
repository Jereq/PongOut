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

bool Block::initialize(const BlockData& _blockData, float _rotation, const std::vector<std::string>& _textures,
						GraphicsComponent::ptr _graphicsComponent)
{
	if(!GameObject::initialize("block", _blockData.center, BLOCKSIZE, _rotation, _graphicsComponent))
		return false;

	for(int i = 0; i < _textures.size(); i++)
	{
		textures.push_back(_textures[i]);
	}

	health = _blockData.health;
	return true;
}

void Block::update(double _dt)
{
	if(canCollide)
		graphicsComponent->addSpriteToFrame(textures[health-1], center, size, rotation);
}
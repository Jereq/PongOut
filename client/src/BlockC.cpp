/*
 * Block.cpp
 *
 *
 *
 */

#include "BlockC.h"
#include <CoreSystem/ICoreSystem.h>

BlockC::BlockC() : GameObject()
{
	
}

BlockC::~BlockC()
{
}

void BlockC::setCollided(const int _health)
{
	if (_health < health)
	{
		ICoreSystem::getInstance().lock()->getSounds()->playSfx("block_explodes");
	}

	health = _health;

	if(health <= 0)
	{
		inPlay = false;
		health = 0;	
	}
}

void BlockC::setInPlay(bool _inPlayState)
{
	GameObject::setInPlay(_inPlayState);
}

bool BlockC::initialize(const CommonTypes::Block& _blockData, float _rotation, GraphicsComponent::ptr _graphicsComponent)
{
	glm::vec3 center(_blockData.pos.x, _blockData.pos.y, 0.f);

	if(!GameObject::initialize(_blockData.id, center, BLOCKSIZE, _rotation, _graphicsComponent))
		return false;

	textures = _blockData.textures;
	health = _blockData.health;
	inPlay = true;
	return true;
}

void BlockC::update(double _dt)
{
	if(health > 0)
		graphicsComponent->addSpriteToFrame(textures[health - 1], center, size, rotation);
}

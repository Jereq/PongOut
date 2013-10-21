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
	health = _health;
	if(health <= 0)
	{
		inPlay = false;
		health = 0;	
	}
	ICoreSystem::getInstance().lock()->getSounds()->playSfx("block_explodes");
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
	health = _blockData.health - 1;
	inPlay = true;
	return true;
}

void BlockC::update(double _dt)
{
	if(inPlay)
		graphicsComponent->addSpriteToFrame(textures[health], center, size, rotation);
}
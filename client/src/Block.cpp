/*
 * Block.cpp
 */

#include "Block.h"

Block::Block()
{
}

Block::~Block()
{
}

void Block::initialize(glm::vec3 _position, glm::vec2 _size, string _texturePath, int _health)
{
	position				= _position;
	size 					= _size;
	texturePath				= _texturePath;
	health 					= _health;

	hitBox = new HitBox(position, size);
}

void Block::setPos(glm::vec3 _position)
{
	position = _position;
}

glm::vec3 Block::getPos()
{
	return position;
}

HitBox* Block::getBox()
{
	return hitBox;
}
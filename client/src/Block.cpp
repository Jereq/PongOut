/*
 * Block.cpp
 *
 *
 *
 */

#include "Block.h"

Block::Block()
{
}

Block::~Block() {}

void Block::initialize(glm::vec2 _position, glm::vec2 _size, string _texturePath, int _health)
{
	position 				= _position;
	size 					= _size;
	texturePath				= _texturePath;
	health 					= _health;
}
void Block::setPos(glm::vec2 _position)
{
	position = _position;
}
glm::vec2 Block::getPos()
{
	return position;
}

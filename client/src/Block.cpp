/*
 * Block.cpp
 *
 *
 *
 */

#include "Block.h"

Block::Block()
{
	position 				= NULL;
	size 					= NULL;
	health 					= NULL;
}

Block::~Block() {}

void Block::initialize(Position _position, Size _size, string _texturePath, int _health)
{
	position 				= _position;
	size 					= _size;
	texturePath				= _texturePath;
	health 					= _health;
}
void Block::setPos(Position _position)
{
	position = _position;
}
Position Block::getPos()
{
	return position;
}

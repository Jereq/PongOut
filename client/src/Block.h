/*
 * Block.h
 *
 *
 *
 */

#ifndef BLOCK_H_
#define BLOCK_H_

#include <iostream>
#include <string>

#include "MapResources.h"

using namespace std;

class Block
{
public:
	Block();
	virtual ~Block();

	void initialize(Position _position, Size _size, string _texturePath, int _health);
	void setPos(Position _position);
	Position getPos();
private:
	Position 			position;
	Size				size;
	string 				texturePath;

	int 				health;


};

#endif /* BLOCK_H_ */

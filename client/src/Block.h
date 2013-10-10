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
#include <CoreSystem/ICoreSystem.h>

using namespace std;

class Block
{
public:
	Block();
	virtual ~Block();

	void initialize(glm::vec2 _position, glm::vec2 _size, string _texturePath, int _health);
	void setPos(glm::vec2 _position);
	glm::vec2 getPos();
private:
	glm::vec2 			position;
	glm::vec2			size;
	string 				texturePath;

	int 				health;


};

#endif /* BLOCK_H_ */

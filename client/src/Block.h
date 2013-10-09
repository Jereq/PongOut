/*
 * Block.h
 */

#ifndef BLOCK_H_
#define BLOCK_H_

#include <iostream>
#include <string>

#include "MapResources.h"
#include <CoreSystem/ICoreSystem.h>
#include "HitBox.h"

using namespace std;

class Block
{
public:
	Block();
	virtual ~Block();

	void initialize(glm::vec3 _position, glm::vec2 _size, string _texturePath, int _health);
	void setPos(glm::vec3 _position);
	glm::vec3 getPos();
	HitBox* getBox();

private:
	glm::vec2			size;
	glm::vec3			position;
	string 				texturePath;

	int 				health;

	HitBox*				hitBox;

};

#endif /* BLOCK_H_ */

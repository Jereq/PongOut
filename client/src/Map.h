/*
 * Map.h
 *
 *  Created on: Sep 17, 2013
 *      Author: victor
 */

#ifndef MAP_H_
#define MAP_H_

#include <malloc.h>
#include <iostream>
#include <string>

#include "Block.h"

using namespace std;

class Map {
public:
	Map();
	virtual ~Map();
	void initialize(	float _playAreaWidth, float _playAreaHeight,
						float _frameThickness, string _texturePath		);
	void update();

private:
	void initBlockArray(int _size);
	void remBlockArray();
	void tempSetBlockPos();

	void setPlayAreaBounds(float _width, float _height);

	Block* 			blockList;
	string 			bgTexturePath;
	string			frameTexturePath;

	float 			playAreaWidth;
	float			playAreaHeight;
	float 			frameThickness;



};

#endif /* MAP_H_ */

/*
 * Map.h
 *
 *
 *
 */

#ifndef MAP_H_
#define MAP_H_

#include <malloc.h>
#include <iostream>
#include <string>

#include "Block.h"
#include "MapResources.h"

using namespace std;

class Map {
public:
	Map();
	virtual ~Map();
	void initialize(	Size _playAreaSize, float _frameThickness,
						string _bgTexturePath, string _frameTexturePath							);
	void update();

private:
	void initBlockArray(int _size);
	void remBlockArray();
	void tempSetBlockPos();

	void setPlayAreaBounds(Size _size);

	Block* 			blockList;
	string 			bgTexturePath;
	string			frameTexturePath;
	Size			playAreaSize;

	float 			playAreaWidth;
	float			playAreaHeight;
	float 			frameThickness;



};

#endif /* MAP_H_ */

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
#include <vector>

#include "Block.h"
#include "MapResources.h"

using namespace std;

class Map {
public:
	Map();
	virtual ~Map();
	void initialize(	glm::vec2 _playAreaSize, float _frameThickness,
						string _bgTextureName, string _frameTextureName							);
	void update();
	string getTextureName();
	string getFrameTextureName();
	glm::vec2 getSize();

private:
	void initBlockArray(int _size);
	void remBlockArray();
	void tempSetBlockPos();

	void setPlayAreaBounds(glm::vec2 _size);

	vector<Block>	blockList;
	string 			bgTextureName;
	string			frameTextureName;
	glm::vec2		playAreaSize;

	float 			frameThickness;



};

#endif /* MAP_H_ */

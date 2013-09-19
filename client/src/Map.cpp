/*
 * Map.cpp
 *
 *  Created on: Sep 17, 2013
 *      Author: victor
 */

#include "Map.h"

Map::Map()
{
	frameThickness 			= NULL;
	playAreaWidth 			= NULL;
	playAreaHeight 			= NULL;
	blockList 				= NULL;
	bgTexturePath 			= NULL;
}

Map::~Map(){}

void Map::initialize(float _playAreaWidth, float _playAreaHeight, float _frameThickness, string _texturePath)
{
	frameThickness 			= _frameThickness;
	bgTexturePath 			= _texturePath;

	setPlayAreaBounds(_playAreaWidth, _playAreaHeight);
	initBlockArray(24); //Hardcoded for demo ONLY!!
}

void Map::setPlayAreaBounds(float _width, float _height)
{
	playAreaWidth = _width + frameThickness;
	playAreaHeight = _height + frameThickness;
}

void Map::initBlockArray(int _size)
{
	blockList = (Block*)malloc(sizeof(Block) * _size);
}
void Map::remBlockArray()
{
	delete [] blockList;
}


/*
 * Map.cpp
 *
 *
 *
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

Map::~Map()
{
	remBlockArray();
}

void Map::initialize(	Size _playAreaSize, float _frameThickness,
						string _bgTexturePath, string _frameTexturePath							)
{
	frameThickness 			= _frameThickness;
	bgTexturePath 			= _bgTexturePath;
	frameTexturePath		= _frameTexturePath;

	setPlayAreaBounds(_playAreaSize);
	initBlockArray(24); //Hardcoded for demo ONLY!!
	tempSetBlockPos();
}

void Map::setPlayAreaBounds(Size _size)
{
	playAreaSize.width 		= _size.width + frameThickness;
	playAreaSize.height		= _size.height + frameThickness;
}

void Map::initBlockArray(int _size)
{
	blockList 				= (Block*)malloc(sizeof(Block) * _size);
	for(int i = 0; i < _size; i++)
	{
		blockList[i] = new Block();
	}
}
void Map::remBlockArray()
{
	delete [] blockList;
}
void Map::tempSetBlockPos()
{
	//Temporary setPos for the blocks, for demo ONLY!
	for(int i = 0; i < 24; i++)
	{
		blockList[i].setPos(Position(i, i));
	}
}

void Map::update()
{
	//Update things here, fo' s'ho.
}

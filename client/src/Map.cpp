/*
 * Map.cpp
 *
 *
 *
 */

#include "Map.h"

Map::Map()
{
	frameThickness 			= 0.f;
	//blockList 				= NULL;

	bgTextureName 			= "NAFINKHERELOLOLOLOL";
}

Map::~Map()
{
	remBlockArray();
}

void Map::initialize(	glm::vec2 _playAreaSize, float _frameThickness,
						string _bgTextureName, string _frameTextureName							)
{
	frameThickness 			= _frameThickness;
	bgTextureName 			= _bgTextureName;
	frameTextureName		= _frameTextureName;

	setPlayAreaBounds(_playAreaSize);
	//initBlockArray(24); //Hardcoded for demo ONLY!!
	tempSetBlockPos();
}

void Map::setPlayAreaBounds(glm::vec2 _size)
{
	playAreaSize.x 		= _size.x + frameThickness;
	playAreaSize.y		= _size.y + frameThickness;
}

void Map::initBlockArray(int _size, GraphicsComponent::ptr _graphicsComponent)
{
	//blockList.resize(_size);
	//blockList[0].initialize(glm::vec2(1,1), glm::vec2(8,4), "blocks/orange_01", 1);

	int screenWidth = playAreaSize.x;
	int screenHeight = playAreaSize.y;
	glm::vec2 size = glm::vec2(64,32);
	glm::vec3 origo = glm::vec3(playAreaSize.x / 2 + size.x / 2., playAreaSize.y/2, 0);
	
	int columns = 10;
	int rows = 3;
	blockList.resize(columns * rows);

	glm::vec3 startPosition = origo;
	for(int i = 0; i < blockList.size(); i++)
	{
		int y = i % columns;
		if(y == 0)
		{
			startPosition.x = origo.x;
			startPosition.y += size.y;
		}

		blockList[i].initialize("UNDEFINED", startPosition, size, 0, _graphicsComponent);
		startPosition.x += size.x;
	}
	
}
void Map::remBlockArray()
{
	//delete [] blockList;
}
void Map::tempSetBlockPos()
{
	//Temporary setPos for the blocks, for demo ONLY!
	for(int i = 0; i < 24; i++)
	{
		//blockList[i].setPos(glm::vec2(i, i));
	}
}
std::string Map::getTextureName()
{
	return bgTextureName;
}
std::string Map::getFrameTextureName()
{
	return frameTextureName;
}
glm::vec2 Map::getSize()
{
	return playAreaSize;
}
void Map::update()
{
	//Update things here, fo' s'ho.
}

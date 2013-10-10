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

bool Map::addObject(Paddle::ptr _paddle)
{
	for(int i = 0; i < paddles.size(); i++)
	{
		//we found a duplicate
		if(paddles[i]->getId() == _paddle->getId())
			return false;
	}

	//new paddle, add accordingly
	paddles.push_back(_paddle);

	return true;
}

bool Map::addObject(Ball::ptr _ball)
{
	for(int i = 0; i < balls.size(); i++)
	{
		if(balls[i]->getId() == _ball->getId())
			return false;
	}

	balls.push_back(_ball);

	return true;
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
	
	int columns = 5;
	int rows = 3;
	//blocks.resize(columns * rows);

	glm::vec3 startPosition = origo;
	for(int i = 0; i < _size; i++)
	{
		int y = i % columns;
		if(y == 0)
		{
			startPosition.x = origo.x;
			startPosition.y += size.y;
		}

		Block::ptr block = Block::ptr(new Block());
		std::string id = "block" + i;		
		block->initialize(id, startPosition, size, 0, _graphicsComponent);
		blocks.push_back(block);

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
void Map::update(double _dt)
{
	for(GameObject::ptr paddle : paddles)
	{
		paddle->update(_dt);
	}

	for(GameObject::ptr ball : balls)
	{
		ball->update(_dt);
	}

	for(GameObject::ptr block : blocks)
	{
		block->update(_dt);
	}
}

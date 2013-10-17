/*
 * Map.cpp
 *
 *
 *
 */

#include "Map.h"
//#include <iostream>
//#include <boost/filesystem/fstream.hpp>

Map::Map()
{
	frameThickness 			= 0.f;
	//blockList 				= NULL;

	bgTextureName 			= "NAFINKHERELOLOLOLOL";
}

Map::~Map()
{
	//remBlockArray();
}

void Map::initialize(	glm::vec2 _playAreaSize, float _frameThickness,
						string _bgTextureName, string _frameTextureName							)
{
	frameThickness 			= _frameThickness;
	bgTextureName 			= _bgTextureName;
	frameTextureName		= _frameTextureName;

	setPlayAreaBounds(_playAreaSize);

	if(blocks.size() != 0)
		blocks.clear();
	if(balls.size() != 0)
		balls.clear();
	if(paddles.size() != 0)
		paddles.clear();
}

bool Map::addObject(Paddle::ptr _paddle)
{
	for(unsigned int i = 0; i < paddles.size(); i++)
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
	for(unsigned int i = 0; i < balls.size(); i++)
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

struct BDATA
{
	int id;
	std::vector<std::string> textures;
};



bool Map::loadMap(std::string _mapName, GraphicsComponent::ptr gc, InputComponent::ptr ic, PhysicsComponent::ptr pc )
{
	bool result;
	return true;
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
void Map::update(double _dt, IGraphics::ptr _graphics)
{
	glm::vec2 clipArea = glm::vec2( playAreaSize.x / 1280. * 2.f, playAreaSize.y / 1024. * 2.f);
	_graphics->addRectangle(glm::vec3(0,0,0.01), clipArea, 0, level.texture);

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

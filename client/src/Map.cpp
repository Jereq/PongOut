/*
 * Map.cpp
 *
 *
 *
 */

#include "Map.h"
//#include <iostream>
//#include <boost/filesystem/fstream.hpp>
#include "TMLReader.h"

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
						std::string _bgTextureName, std::string _frameTextureName							)
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

inline glm::vec2 screenSizeToClip( const glm::vec2 _screenDimension, const glm::vec2 _screenSize )
{
	return glm::vec2( _screenSize.x / _screenDimension.x * 2.f, _screenSize.y / _screenDimension.y * 2.f);
}
 
inline glm::vec3 screenPositionToClip( const glm::vec2 _screenDimension, const glm::vec3 _screenPosition )
{
	return glm::vec3(_screenPosition.x / _screenDimension.x * 2.f - 1.f, _screenPosition.y / _screenDimension.y * 2.f - 1.f, _screenPosition.z);
}

bool Map::loadMap(std::string _mapName, std::vector<CommonTypes::Block> _blockData, GraphicsComponent::ptr gc, InputComponent::ptr ic, PhysicsComponent::ptr pc )
{
	Paddle::ptr p = Paddle::ptr(new Paddle);
	p->initialize("paddle", glm::vec3(0, 100, 0), glm::vec2(128, 32), 0, gc, ic, pc);
	paddles.push_back(p);

	p = Paddle::ptr(new Paddle);
	p->initialize("paddle1", glm::vec3(0,800,0), glm::vec2(128, 32), 0, gc, ic, pc);
	paddles.push_back(p);

	Ball::ptr b = Ball::ptr(new Ball);
	b->initialize("ball", glm::vec3(0, 800, 0), glm::vec2(32,32), 0, gc, pc);
	balls.push_back(b);

	b = Ball::ptr(new Ball);
	b->initialize("ball", glm::vec3(0, 0, 0), glm::vec2(32,32), 0, gc, pc);
	balls.push_back(b);

	for(CommonTypes::Block bd : _blockData)
	{
		BlockC::ptr block = BlockC::ptr(new BlockC);
		block->initialize(bd, 0, gc);
		blocks.push_back(block);
	}

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
	_graphics->addRectangle(glm::vec3(0,0,0.01), clipArea, 0, bgTextureName);

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

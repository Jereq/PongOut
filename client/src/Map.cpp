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

		TMLReader tml;
	tml.readFile("../test.tml");

	int MapId = 1;

	// Fetch the nodes of interest
	TMLNODE r = tml.getRoot();
	TMLNODE mapNode, blockNode;
	for( TMLNODE n : tml.getRoot().nodes )
	{
		if(n.name == "Map")
		{
			for(std::pair<std::string,std::string> m : n.members)
			{
				if(m.first == "id")
				{
					int id = atoi(m.second.c_str());
					if(id == MapId)
					{
						mapNode = n;
					}
				}
			}
		}
		if(n.name == "Blocks")
		{
			blockNode = n;
		}
	}

	// fetch map info
	int id;
	std::string name;
	std::vector<std::string> rows;
	for(std::pair<std::string,std::string> m : mapNode.members)
	{
		if(m.first == "id")
		{
			id = atoi(m.second.c_str());
		}
		else if(m.first == "name")
		{
			name = m.second;
		}
		else if(m.first == "row")
		{
			rows.push_back(m.second);
		}
	}

	// create blocks
	int offX = 0;
	int offY = 0;
	for(std::string row : rows )
	{
		for(int i = 0; i < row.size(); i++)
		{
			std::string val = row.substr(i,1);
			if(val == "0")
				continue;

			BlockData bd;
			bd.health = 0;
			std::vector<std::string> textures;
			for(TMLNODE n : blockNode.nodes)
			{
				std::string id = "-1";
				for(std::pair<std::string,std::string> m : n.members)
				{
					
					if(m.first == "id")
					{
						id = m.second;
						if(id == val)
						{
							int x,y;
							x = 880 / 2 + BLOCKSIZE.x * i;
							y = 800 / 2 + offY * BLOCKSIZE.y;
							printf("{%i %i}",x,y);
			
							bd.center = glm::vec3(x,y, 0);
						}
					}
					if(m.first == "texture" && id == val)
					{
						textures.push_back(m.second);
					}
				}
			}
			bd.health = textures.size();
			Block::ptr b = Block::ptr(new Block);
			b->initialize(bd,0,textures, gc);
			blocks.push_back(b);
			textures.clear();
		}
		printf("\n");
		offY++;
	}

	Paddle::ptr p = Paddle::ptr(new Paddle);
	p->initialize("paddle", glm::vec3(0, 100, 0), glm::vec2(128, 43), 0, gc, ic, pc);
	paddles.push_back(p);

	Ball::ptr b = Ball::ptr(new Ball);
	b->initialize("ball", glm::vec3(0, 800, 0), glm::vec2(32,32), 0, gc, pc);
	balls.push_back(b);
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

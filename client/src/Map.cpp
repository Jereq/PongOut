/*
 * Map.cpp
 *
 *
 *
 */

#include "Map.h"
#include <iostream>
#include <boost/filesystem/fstream.hpp>
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

struct BlockType
{
	int id;
	int health;
	std::vector<std::string> textures;
};

struct Level
{
	int id;
	std::string name;
	std::string texture;

	std::vector<Block> rows;

};

bool readMapFile(std::vector<Block::ptr>& _blocks, const std::string& levelName, const glm::vec2& _playAreaSize, GraphicsComponent::ptr _graphicsComponent)
{
	boost::filesystem::fstream file("../maps.txt");
	
	if(file == NULL)
	{
		return false;
	}

	std::vector<BlockType> blockTypes;
	glm::vec2 size = glm::vec2(64,32);
	glm::vec3 origo = glm::vec3(_playAreaSize.x / 2. - size.x, _playAreaSize.y-size.y, 0);

	while(true)
	{
		std::string line;

		if(!std::getline(file,line))
			break;

		std::getline(file,line);
		
		BlockType block;
		int id = 0, health = 0;
		block.id = id;
		block.health = 0;

		Level level;
		int row = 0;
		while(true)
		{
			
			int sep = line.find_first_of(' ');
			int end = line.size();
			if(line == "/def")
				break;

			if(line.substr(0, sep) == "\tid")
			{
				block.id = atoi(line.substr(sep+1,end).c_str());
			}
			else if(line.substr(0, sep) == "\thealth")
			{
				block.health = atoi(line.substr(sep+1,end).c_str());
			}
			else if(line.substr(0, sep) == "\ttexture")
			{
				block.textures.push_back(line.substr(sep+1,end));
			}
			else if(line.substr(0, sep) == "\t/end")
			{
				blockTypes.push_back(block);
				block.textures.clear();
			}


			if(line.substr(0, sep) == "\tid")
			{
				level.id = atoi(line.substr(sep,end).c_str());
			}
			else if(line.substr(0, sep) == "\tname")
			{
				level.name = line.substr(sep+1,end);
			}
			else if(line.substr(0, sep) == "\ttexture")
			{
				level.texture = line.substr(sep+1,end);
			}
			else if(line.substr(0, sep) == "\trow")
			{
				std::string idTypes = line.substr(sep+1,end);

				for(int i = 0; i < idTypes.size(); i++)
				{
					std::string val = idTypes.substr(i,1);
					if(val == "0")
						continue;

					float x = origo.x + i * size.x;
					printf("{%f %f %f}\n",x , origo.y + row * size.y, 0);
					glm::vec3 position = glm::vec3(x, origo.y - row * size.y, 0);
					Block::ptr b = Block::ptr(new Block());
					int id = atoi(val.c_str());
					std::string texture = "";
					for(BlockType btype : blockTypes)
					{
						if(id == btype.id)
							texture = btype.textures[0];
					}


					b->initialize("block", position, size, 0, _graphicsComponent, texture);
					_blocks.push_back(b);
				}
				printf("---\n");
				row++;
			}
			std::getline(file, line);
		}
	}

	blockTypes.clear();
	file.close();

	return true;
};

bool Map::loadMap(std::string _mapName, GraphicsComponent::ptr gc, InputComponent::ptr ic, PhysicsComponent::ptr pc )
{
	bool result;
	result = readMapFile(blocks, _mapName, playAreaSize, gc);	

	Ball::ptr b = Ball::ptr(new Ball());
	result = b->initialize("ball1", glm::vec3(600,400,-0.1), glm::vec2(32,32), 0, GraphicsComponent::ptr(gc), PhysicsComponent::ptr(pc));
	addObject(b);

	Paddle::ptr p = Paddle::ptr(new Paddle());
	result = p->initialize("paddle1", glm::vec3(400,100,0), glm::vec2(128,32), 0, GraphicsComponent::ptr(gc), InputComponent::ptr(ic), PhysicsComponent::ptr(pc));
	addObject(p);

	return result;
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

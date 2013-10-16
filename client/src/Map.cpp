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





bool readBlockTypes(std::vector<BlockType>& _blockTypes, std::istream& _is)
{
		BlockType block;
		int id = 0, health = 0;
		block.id = id;
		block.health = 0;

	while(true)
	{
		std::string line;
		std::getline(_is, line);

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
			_blockTypes.push_back(block);
			block.textures.clear();
		}
	}

	return true;
}

bool readMap(std::vector<BlockData>& _blockData, const std::vector<BlockType>& _blockTypes, Level& _level, std::istream& _is)
{
	std::string name;
	glm::vec2 size = glm::vec2(64,32);
	int row = 0;
	while(true)
	{
		std::string line;
		std::getline(_is, line);

		int sep = line.find_first_of(' ');
		int end = line.size();
			if(line == "/def")
				break;

		if(line.substr(0, sep) == "\tname")
		{
			name = line.substr(sep+1,end);
		}
		else if(line.substr(0, sep) == "\ttexture")
		{
			if(name == _level.name)
				_level.texture = line.substr(sep+1,end);
		}
		else if(line.substr(0, sep) == "\trow" && name == _level.name)
		{
			std::string idTypes = line.substr(sep+1,end);

			for(int i = 0; i < idTypes.size(); i++)
			{
				std::string val = idTypes.substr(i,1);
				if(val == "0")
					continue;

				BlockData bd;
				bd.typeId = atoi(val.c_str());
				bd.center = glm::vec3(_level.origo.x + i * size.x, 
												_level.origo.y - row * size.y, 0);
				
				_blockData.push_back(bd);
			}
			row++;
		}
	}
	return true;
}

bool readMapFile(std::vector<Block::ptr>& _blocks, Level& _level, const glm::vec2& _playAreaSize, GraphicsComponent::ptr _graphicsComponent)
{
	boost::filesystem::fstream file("../maps.txt");	
	if(file == NULL)
	{
		return false;
	}

	std::vector<BlockType> blockTypes;	// type of block
	std::vector<BlockData> blockData;	// none-CoreSystem-dependent block data
	
	// read in block types and requested map
	while(true)
	{
		std::string line;

		if(!std::getline(file,line))
			break;

		int sep = line.find_first_of(' ');
		int end = line.size();

		if(line == "def Block")	// create types of blocks
		{
			readBlockTypes(blockTypes, file);
		}
		else if(line == "def Map")	// create level
		{
			readMap(blockData, blockTypes, _level, file);
		}
	}
	file.close();

	// initialize the blocks
	for(unsigned int i = 0; i < blockData.size(); i++)
	{
		if(blockData[i].typeId == 0)
			continue;

		Block::ptr b = Block::ptr(new Block());

		int typeId = 0;
		for(int j = 0; j < blockTypes.size(); j++)
		{
			if(blockData[i].typeId == blockTypes[j].id)
				typeId = j;
		}

		b->initialize(blockData[i], 0, blockTypes[typeId].textures, _graphicsComponent);
		_blocks.push_back(b);

	}
	blockTypes.clear();
	blockData.clear();

	return true;
};

bool Map::loadMap(std::string _mapName, GraphicsComponent::ptr gc, InputComponent::ptr ic, PhysicsComponent::ptr pc )
{
	bool result;
	level.name = _mapName;
	level.origo = glm::vec3(playAreaSize.x / 2. - BLOCKSIZE.x, playAreaSize.y-BLOCKSIZE.y, 0);
	result = readMapFile(blocks, level, playAreaSize, gc);	

	Ball::ptr b = Ball::ptr(new Ball());
	result = b->initialize("ball1", glm::vec3(600,400,-0.1), glm::vec2(32,32), 0, GraphicsComponent::ptr(gc), PhysicsComponent::ptr(pc));
	addObject(b);

	Paddle::ptr p = Paddle::ptr(new Paddle());
	result = p->initialize("paddle1", glm::vec3(400,100,0), glm::vec2(128,32), 0, GraphicsComponent::ptr(gc), InputComponent::ptr(ic), PhysicsComponent::ptr(pc));
	addObject(p);

	return result;
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

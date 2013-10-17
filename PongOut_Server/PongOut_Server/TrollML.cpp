#include "stdafx.h"
#include "TrollML.h"

#include <iostream>
#include <boost/filesystem/fstream.hpp>

TrollML::TrollML(void)
{
}


TrollML::~TrollML(void)
{
}

bool TrollML::readFile(int _mapID, Map& _map) //UGLIES CODE EVER WRITTEN BY MAN!! 
{
	boost::filesystem::fstream file("../maps.txt");	

	if(file == NULL)
	{
		return false;
	}

	std::vector<GameMessage::Block> blockTypes;	// type of block
	std::vector<GameMessage::Block> blockData;	// none-CoreSystem-dependent block data

	for (ever)
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
			readMap(blockTypes, blockData, file, _mapID);
		}
	}
	file.close();

	for(unsigned int i = 0; i < blockData.size(); i++)
	{
		if(blockData[i].id == 0)
			continue;		

		int typeId = 0;
		for(int j = 0; j < blockTypes.size(); j++)
		{
			if(blockData[i].id == blockTypes[j].id)
				typeId = j;
		}

		GameMessage::Block b;

		b.health = blockTypes[typeId].textures.size();    //TROLOLOLOLOLOL
		b.id = blockData[i].id;
		b.textures = blockTypes[typeId].textures;
		b.x = blockData[i].x;
		b.y = blockData[i].y;
		b.z = blockData[i].z;

		_map.setMapData(b);

	}
	blockTypes.clear();

	return true;
}

bool TrollML::readBlockTypes(std::vector<GameMessage::Block>& _blockTypes, std::istream& _is)
{
	GameMessage::Block block;
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

bool TrollML::readMap(std::vector<GameMessage::Block>& _blockTypes, std::vector<GameMessage::Block>& _blockData, std::istream& _is, int _mapId)
{
	std::string name;
	int id;
	int row = 0;
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
			id = atoi(line.substr(sep+1,end).c_str());
		}
		if(line.substr(0, sep) == "\tname")
		{
			name = line.substr(sep+1,end);
		}
		else if(line.substr(0, sep) == "\ttexture")
		{
			//if(name == _level.name)
			//	_level.texture = line.substr(sep+1,end);
		}
		else if(line.substr(0, sep) == "\trow" && id == _mapId)
		{
			std::string idTypes = line.substr(sep+1,end);

			for(unsigned int i = 0; i < idTypes.size(); i++)
			{
				std::string val = idTypes.substr(i,1);
				if(val == "0")
					continue;

				GameMessage::Block bd;
				bd.id = atoi(val.c_str());
				bd.x = 800.0f / 2.0f - 64 * i + 64;
				bd.y = 800.0f / 2.0f - 32 - row * 32;

				_blockData.push_back(bd);
			}
			row++;
		}
	}
	return true;
}

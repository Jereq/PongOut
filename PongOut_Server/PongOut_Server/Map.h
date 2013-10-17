#pragma once

#include <vector>

#include <GameMessage.h>

class Map
{
public:

	Map(void);
	~Map(void);

	void setMapData(const GameMessage::Block& _block);
	void setMapName(std::string _name);
	
private:

	std::vector<GameMessage::Block> blockList;
	
};


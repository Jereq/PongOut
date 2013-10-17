#pragma once

#include <vector>

#include <GameMessage.h>

class Map
{
public:

	Map(void);
	~Map(void);

	void setMapData(const CommonTypes::Block& _block);
	void setMapName(std::string _name);
	
private:

	std::vector<CommonTypes::Block> blockList;
	
};


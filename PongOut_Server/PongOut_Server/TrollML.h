#pragma once

#include "Map.h"

class TrollML
{
public:

	TrollML(void);
	~TrollML(void);

	bool readFile(int _mapID, Map& _map);

private:

	bool readBlockTypes(std::vector<GameMessage::Block>& _blockTypes, std::istream& _is);
	bool readMap(std::vector<GameMessage::Block>& _blockTypes, std::vector<GameMessage::Block>& _blockData, std::istream& _is, int _mapId);
};
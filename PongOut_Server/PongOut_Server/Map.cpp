#include "stdafx.h"
#include "Map.h"


Map::Map(void)
{
}


Map::~Map(void)
{
}

void Map::setMapData( const CommonTypes::Block& _block )
{
	blockList.push_back(_block);
}

#include "stdafx.h"
#include "GameMaster.h"

GameMaster& GameMaster::getInstance()
{
	static GameMaster instance;
	return instance;
}

GameMaster::GameMaster(void)
{
}


GameMaster::~GameMaster(void)
{
}

void GameMaster::handleGameMessage( GameMessage::ptr _message )
{
	
}

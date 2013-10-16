#include "stdafx.h"
#include "GameMaster.h"
#include "GameMessage.h"

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
	switch (_message->getGameType())
	{
	case GameMessage::GameMsgType::INITGAMEREQUEST:
		{
			break;
		}
	default:
		{
			break;
		}
	}
}

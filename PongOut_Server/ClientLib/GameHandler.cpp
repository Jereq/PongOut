#include "stdafx.h"
#include "GameHandler.h"

GameHandler& GameHandler::getInstance()
{
	static GameHandler instance;
	return instance;
}

GameHandler::GameHandler(void)
{
}

GameHandler::~GameHandler(void)
{
}

void GameHandler::handleGameMessage( GameMessage::ptr _gmp )
{
	switch (_gmp->getGameType())
	{
	case GameMessage::GameMsgType::CREATEGAMERESPONSE:
		{
			break;
		}
	default:
		break;
	}
}

#include "stdafx.h"
#include "GameHandler.h"
#include "CreateGameResponse.h"

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

void GameHandler::handleGameMessage( GameMessage::ptr _gmp, SafeQueue<message>& _msgQueue )
{
	switch (_gmp->getGameType())
	{
	case GameMessage::GameMsgType::CREATEGAMERESPONSE:
		{
			CreateGameResponse::ptr cgrp = boost::static_pointer_cast<CreateGameResponse>(_gmp);
			
			_msgQueue.push(message(GameMessage::GameMsgType::CREATEGAMERESPONSE, cgrp));
			break;
		}
	}
}
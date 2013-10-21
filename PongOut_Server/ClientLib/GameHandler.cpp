#include "stdafx.h"
#include "GameHandler.h"
#include "CreateGameResponse.h"
#include "GameTickUpdate.h"

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
	case GameMessage::GameMsgType::GAME_TICK_UPDATE:
	case GameMessage::GameMsgType::END_GAME_RESPONSE:
		{
			_msgQueue.push(message(_gmp->getGameType(), _gmp));
			break;
		}

	default:
		_msgQueue.push(message(msgBase::MsgType::INTERNALMESSAGE, "Unhandled game message received"));
	}
}

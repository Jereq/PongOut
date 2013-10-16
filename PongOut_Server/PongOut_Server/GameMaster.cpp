#include "stdafx.h"
#include "GameMaster.h"
#include "GameMessage.h"
#include "UserManager.h"
#include "Referee.h"

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

void GameMaster::handleGameMessage( GameMessage::ptr _message, User::ptr _initUser )
{
	switch (_message->getGameType())
	{
	case GameMessage::GameMsgType::INITGAMEREQUEST:
		{
			CreateGame::ptr cgp = boost::static_pointer_cast<CreateGame>(_message);

			Referee r;

			std::unique_lock<std::mutex> lock(UserManager::getInstance()->users.getLock());
			User::ptr opponentUser;

			for (User::ptr user : UserManager::getInstance()->users.baseSet)
			{
				if (user->getUserID() != _initUser->getUserID())
				{
					opponentUser = user;
					break;
				}
			}
			
			r.init(_initUser, opponentUser, cgp->getInitInfo());

			break;
		}
	default:
		{
			break;
		}
	}
}

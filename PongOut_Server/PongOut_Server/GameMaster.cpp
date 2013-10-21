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
	refIDCounter = 0;
}


GameMaster::~GameMaster(void)
{
}

void GameMaster::handleGameMessage( GameMessage::ptr _message, User::ptr _initUser )
{
	switch (_message->getGameType())
	{
	case GameMessage::GameMsgType::CREATEGAMEREQUEST:
		{
			_initUser->setUserState(User::UserState::WATING_FOR_OPPONENT);

			CreateGameRequest::ptr cgp = boost::static_pointer_cast<CreateGameRequest>(_message);

			Referee::ptr r = Referee::ptr(new Referee());

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
			
			r->init(_initUser, opponentUser, cgp->getInitInfo(), refIDCounter);
			refIDCounter++;

			referees.insert(std::pair<int, Referee::ptr>(refIDCounter, r));

			break;
		}
	case GameMessage::GameMsgType::PADDLEUPDATEREQUEST:
		{
			referees.at(_initUser->getRefereeID())->addMsgToQ(std::make_pair(_message, _initUser->getUserID()));

			break;
		}

	default:
		{
			break;
		}
	}
}

void GameMaster::removeReferee( int _key )
{
	referees.erase(_key);
}

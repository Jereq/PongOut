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

			std::unique_lock<std::mutex> lock(UserManager::getInstance()->users.getLock());
			User::ptr opponentUser;

			for (User::ptr user : UserManager::getInstance()->users.baseSet)
			{
				if (user->getUserID() != _initUser->getUserID() && user->getUserState() == User::UserState::AVAILABLE)
				{
					opponentUser = user;
					break;
				}
			}

			if (!opponentUser)
			{
				Log::addLog(Log::LogType::LOG_INFO, 3, "Game hosted with no available opponent", __FILE__, __LINE__);
				_initUser->setUserState(User::UserState::AVAILABLE);
				return;
			}

			Referee::ptr r = Referee::ptr(new Referee());
			r->init(_initUser, opponentUser, cgp->getInitInfo(), refIDCounter);
			referees.insert(std::pair<int, Referee::ptr>(refIDCounter, r));
			
			Log::addLog(Log::LogType::LOG_INFO, 4, "Created new game between " + std::to_string(_initUser->getUserID()) + " and " + std::to_string(opponentUser->getUserID()) + " with referee " + std::to_string(refIDCounter), __FILE__, __LINE__);

			refIDCounter++;
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

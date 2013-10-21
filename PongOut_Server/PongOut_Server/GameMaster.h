#pragma once

#include <map>

#include "GameMessage.h"
#include "Referee.h"
#include "SafeMap.h"

class GameMaster
{
public:

	static GameMaster& getInstance();

	void handleGameMessage(GameMessage::ptr _message, User::ptr _initUser);
	void removeReferee(int _key);

private:

	GameMaster(void);
	~GameMaster(void);
	
	SafeMap<int, Referee::ptr> referees;

	int refIDCounter;
};


#pragma once

#include <map>

#include "GameMessage.h"
#include "Referee.h"

class GameMaster
{
public:

	static GameMaster& getInstance();

	void handleGameMessage(GameMessage::ptr _message, User::ptr _initUser);

private:

	GameMaster(void);
	~GameMaster(void);
	
	std::map<int, Referee::ptr> referees;

	int refIDCounter;
};


#pragma once

#include <vector>

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
	
	std::vector<Referee> referees;
	int refIDCounter;
};


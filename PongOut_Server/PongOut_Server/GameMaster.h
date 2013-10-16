#pragma once

#include <vector>

#include "GameMessage.h"
#include "Referee.h"

class GameMaster
{
public:

	static GameMaster& getInstance();

	void handleGameMessage(GameMessage::ptr _message);

private:

	GameMaster(void);
	~GameMaster(void);
	
	std::vector<Referee> referees;
};


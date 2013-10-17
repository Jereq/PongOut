#pragma once

#include "GameMessage.h"

class GameHandler
{
public:

	static GameHandler& getInstance();

	void handleGameMessage(GameMessage::ptr _gmp);

private:

	GameHandler(void);
	~GameHandler(void);
};


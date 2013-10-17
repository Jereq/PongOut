#pragma once

#include "GameMessage.h"
#include "SafeQueue.h"
#include "message.h"

class GameHandler
{
public:

	static GameHandler& getInstance();

	void handleGameMessage(GameMessage::ptr _gmp, SafeQueue<message>& _msgQueue);

private:

	GameHandler(void);
	~GameHandler(void);
};


#pragma once

#include "ICoreSystem.h"

class Game
{
private:
	ICoreSystem* system;

public:
	Game(ICoreSystem* _system);

	void run();
};

#pragma once

#include "ICoreSystem.h"
#include "Map.h"
//#include "MapResources.h"

/**
 * Main game class responsible for managing screens and the game loop.
 */
class Game
{
private:
	ICoreSystem* system;
	Map* map;

public:
	/**
	 *
	 */
	Game(ICoreSystem* _system);

	void run();
};

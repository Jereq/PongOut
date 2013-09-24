#pragma once

#include "ICoreSystem.h"
#include "Map.h"
//#include "MapResources.h"

/**
 *  Main game class responsible for managing screens and the game loop.
 */
class Game
{
private:
	ICoreSystem* system;
	Map* map;

public:
	/** constructor.
	 *
	 *  @param _system the platform dependent core system to be used by the game
	 */
	Game(ICoreSystem* _system);

	/** Start the game.
	 *
	 *  Start the game running. Should return once the game has exited.
	 */
	void run();
};

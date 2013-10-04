#pragma once

#include "ICoreSystem.h"
#include "ScreenManager.h"

#include "Map.h"
//#include "MapResources.h"

/**
 *  Main game class responsible for managing screens and the game loop.
 */
class Game
{
private:
	ICoreSystem::ptr	system;
	Map*				map;
	ScreenManager		screenManager;
public:
	/** constructor.
	 *
	 *  @param _system the platform dependent core system to be used by the game
	 */
	Game(ICoreSystem::ptr _system);

	/** Start the game.
	 *
	 *  Start the game running. Should return once the game has exited.
	 */
	void run();
};

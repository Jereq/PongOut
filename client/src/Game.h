#pragma once

#include "FunctionHandler.h"
#include "ScreenManager.h"
#include <CoreSystem/ICoreSystem.h>

#include "Map.h"
//#include "MapResources.h"

/**
 *  Main game class responsible for managing screens and the game loop.
 */
class Game : public FunctionHandler
{
private:
	ICoreSystem::ptr	system;
	Map*				map;
	ScreenManager		screenManager;
	bool				shouldStop;

	void onFunction(const std::string& _func) override;

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

	/** Stop the game.
	 *
	 *  Stops the game from running. Will not take effect until the current update is done.
	 */
	void stop();
};

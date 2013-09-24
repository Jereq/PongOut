#pragma once

#include "ICoreSystem.h"

/**
 * Main game class responsible for managing screens and the game loop.
 */
class Game
{
private:
	ICoreSystem* system;

public:
	/**
	 *
	 */
	Game(ICoreSystem* _system);

	void run();
};

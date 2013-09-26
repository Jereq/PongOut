#include "Game.h"
#include "ICoreSystem.h"

int main(int argc, char* argv[])
{
	ICoreSystem* system = ICoreSystem::createInstance(argc, argv);
	Game game(system);
	game.run();

	delete system;

	return 0;
}

#include "Game.h"
#include "ICoreSystem.h"

int main(int argc, char* argv[])
{
	ICoreSystem::init(argc, argv);

	Game game(ICoreSystem::getInstance());
	game.run();

	ICoreSystem::destroy();

	return 0;
}

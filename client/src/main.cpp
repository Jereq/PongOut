#include "Game.h"
#include "ICoreSystem.h"

//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

int main(int argc, char* argv[])
{
	//_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
 //   _CrtSetBreakAlloc(9554);
 //   _CrtSetBreakAlloc(9553);
 //   _CrtSetBreakAlloc(9552);
	ICoreSystem::init(argc, argv);

	Game game(ICoreSystem::getInstance());
	game.run();

	ICoreSystem::destroy();

	return 0;
}

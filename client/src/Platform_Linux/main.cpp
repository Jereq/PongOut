#include "Game.h"
#include "Platform_Linux/CoreSystemLinux.h"

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include <iostream>

int main(int argc, char* argv[])
{
	fs::path fullPath(fs::initial_path<fs::path>());
	fullPath = fs::system_complete(fs::path(argv[0]));

	if (!fs::exists(fullPath))
	{
		std::cout << "Error: Invalid argv[0], stop hacking!" << std::endl;
		return 1;
	}

	CoreSystemLinux system(fullPath.parent_path());
	Game game(&system);
	game.run();

	return 0;
}

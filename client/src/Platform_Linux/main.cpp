#include "Graphics.h"
#include "PongOutConfig.h"

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include <chrono>
#include <iostream>
#include <thread>

int main(int argc, char* argv[])
{
	fs::path fullPath(fs::initial_path<fs::path>());
	fullPath = fs::system_complete(fs::path(argv[0]));
	std::cout << fullPath << std::endl;

	Graphics::rootDir = fullPath.parent_path();

	if (!fs::exists(fullPath))
	{
		std::cout << "Error: Invalid argv[0], stop hacking!" << std::endl;
		return 1;
	}

	std::cout << "PongOut " << PongOut_VERSION_MAJOR << "." << PongOut_VERSION_MINOR << "." << PongOut_VERSION_PATCH << std::endl;

	Graphics g;
	if (!g.init())
	{
		std::cout << "Failed to initialize graphics." << std::endl;
		return 1;
	}

	g.loadImage(Graphics::rootDir / "resources/box_orange_01.png");

	float previousTime = (float) g.getTime();
	float currentTime = previousTime;
	float deltaTime = 0.f;

	std::cout << "Starting to run" << std::endl;

	while (!g.windowClosing())
	{
		g.pollEvents();

		const static float FRAME_TIME = 1.f / 60.f;
		deltaTime = currentTime - previousTime;
		if (deltaTime < FRAME_TIME)
		{
			std::chrono::milliseconds dura((unsigned int)((FRAME_TIME - deltaTime) * 1000.f));
			std::this_thread::sleep_for(dura);
		}

		g.draw();
	}

	g.destroy();

	return 0;
}

#include "Game.h"

#include "PongOutConfig.h"

//#include <chrono>
#include <iostream>
#include <stdio.h>
//#include <thread>

Game::Game(ICoreSystem* _system)
	: system(_system)
{
	
}
void Game::run()
{
	std::cout << "PongOut " << PongOut_VERSION_MAJOR << "." << PongOut_VERSION_MINOR << "." << PongOut_VERSION_PATCH << std::endl;

	IGraphics* graphics = system->getGraphics();
	if (!graphics->init())
	{
		printf("Failed to initialize renderer\n");
		return;
	}

	graphics->loadResources(system->getRootDir() / "resources");

	double previousTime = system->getTime();
	double currentTime = previousTime;
	double deltaTime = 0.f;

	std::cout << "Starting to run" << std::endl;

	while (!system->windowIsClosing())
	{
		system->pollEvents();

		graphics->addRectangle(glm::vec3(0.5f, 0.5f, 0.f), glm::vec2(0.5f, 0.5f), 0.f, "blocks/pink_01");
		graphics->addRectangle(glm::vec3(-0.5f, 0.f, 0.f), glm::vec2(1.f), 0.f, "blocks/orange_01");
		graphics->addRectangle(glm::vec3(0.f, 0.f, -1.f), glm::vec2(0.5f), 0.f, "player/ball");

		const static float FRAMES_PER_SECOND = 60.f;
		const static float FRAME_TIME = 1.f / FRAMES_PER_SECOND;

		previousTime = currentTime;
		currentTime = system->getTime();
		deltaTime = currentTime - previousTime;

		if (deltaTime < FRAME_TIME)
		{
			//std::chrono::milliseconds dura((unsigned int)((FRAME_TIME - deltaTime) * 1000.f));
			//std::this_thread::sleep_for(dura);
		}

		graphics->drawFrame();
	}

	graphics->destroy();
}

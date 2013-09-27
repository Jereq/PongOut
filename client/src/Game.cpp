#include "Game.h"

#include "PongOutConfig.h"

//#include <chrono>
#include <iostream>
#include <stdio.h>
//#include "Map.h"

//#include <thread>

Game::Game(ICoreSystem::ptr _system)
	: system(_system)
{
	map = new Map();
	map->initialize(glm::vec2(800.0f, 600.0f), 8, "background/mainmenu_01", "blocks/orange_01");
}
void Game::run()
{
	std::cout << "PongOut " << PongOut_VERSION_MAJOR << "." << PongOut_VERSION_MINOR << "." << PongOut_VERSION_PATCH << std::endl;

	std::shared_ptr<ICoreSystem> systemPtr(system);

	IGraphics::ptr graphics = systemPtr->getGraphics();
	if (!graphics->init())
	{
		printf("Failed to initialize renderer\n");
		return;
	}

	graphics->loadResources(systemPtr->getRootDir() / "resources");

	double previousTime = systemPtr->getTime();
	double currentTime = previousTime;
	double deltaTime = 0.f;

	std::cout << "Starting to run" << std::endl;
	std::cout << "Texture name: \n" << map->getTextureName() << std::endl;


	while (!systemPtr->windowIsClosing())
	{
		systemPtr->pollEvents();

		graphics->addRectangle(glm::vec3(0.f, 0.f, 0.f), glm::vec2(2.f), 0.f, map->getTextureName());

		graphics->addRectangle(glm::vec3(-1.f, 0.f, 0.f), glm::vec2(0.05, 2.f), 0.f, map->getFrameTextureName());
		graphics->addRectangle(glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.05, 2.f), 0.f, map->getFrameTextureName());
		graphics->addRectangle(glm::vec3(0.f, -1.f, 0.f), glm::vec2(2.f, 0.05), 0.f, map->getFrameTextureName());
		graphics->addRectangle(glm::vec3(0.f, 1.f, 0.f), glm::vec2(2.f, 0.05), 0.f, map->getFrameTextureName());

		//graphics->addRectangle(glm::vec3(0.f, 0.f, 0.f), glm::vec2(1.f), 0.f, "blocks/orange_01");
		//graphics->addRectangle(glm::vec3(0.5f, 0.5f, 0.f), glm::vec2(0.5f, 0.5f), 0.f, "blocks/pink_01");
		//graphics->addRectangle(glm::vec3(-0.5f, 0.f, 0.f), glm::vec2(1.f), 0.f, "blocks/orange_01");
		//graphics->addRectangle(glm::vec3(0.f, 0.f, -1.f), glm::vec2(0.5f), 0.f, "player/ball");

		const static float FRAMES_PER_SECOND = 60.f;
		const static float FRAME_TIME = 1.f / FRAMES_PER_SECOND;

		previousTime = currentTime;
		currentTime = systemPtr->getTime();
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


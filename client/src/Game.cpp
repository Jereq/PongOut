#include "Game.h"

#include "PongOutConfig.h"
//#include <chrono>
#include <iostream>
#include <stdio.h>

//#include <thread>
#include <vector>

void Game::onFunction(const std::string& _func)

{
	std::cout << "Performing: " << _func << std::endl;

	if (_func == "exit")
	{
		stop();
	}
	else if (_func == "login")
	{
		std::string username = screenManager.getText("username");
		std::string password = screenManager.getText("password");

		if (username == "test" && password == "pass")
		{
			screenManager.openScreen("mainmenu");
		}
	}
}

Game::Game(ICoreSystem::ptr _system)
	: system(_system),
	screenManager(_system.lock()->getRootDir(), this),
	shouldStop(false)
{
	//map = new Map();
	//map->initialize(glm::vec2(800.0f, 600.0f), 8, "background/mainmenu_01", "blocks/orange_01");
}

static std::string naiveUTF32toUTF8(char32_t _character)
{
	char buffer[6];

	if (_character <= 0x7F)
	{
		return std::string(1, (char)_character);
	}
	else if (_character <= 0x07FF)
	{
		buffer[1] = (_character & 0x3F) | 0x80;
		_character >>= 6;
		buffer[0] = _character | 0xC0;

		return std::string(buffer, buffer + 2);
	}
	else if (_character <= 0xFFFF)
	{
		buffer[2] = (_character & 0x3F) | 0x80;
		_character >>= 6;
		buffer[1] = (_character & 0x3F) | 0x80;
		_character >>= 6;
		buffer[0] = _character | 0xE0;

		return std::string(buffer, buffer + 3);
	}
	else if (_character <= 0x1FFFFF)
	{
		buffer[3] = (_character & 0x3F) | 0x80;
		_character >>= 6;
		buffer[2] = (_character & 0x3F) | 0x80;
		_character >>= 6;
		buffer[1] = (_character & 0x3F) | 0x80;
		_character >>= 6;
		buffer[0] = _character | 0xF0;

		return std::string(buffer, buffer + 4);
	}
	else if (_character <= 0x3FFFFFF)
	{
		buffer[4] = (_character & 0x3F) | 0x80;
		_character >>= 6;
		buffer[3] = (_character & 0x3F) | 0x80;
		_character >>= 6;
		buffer[2] = (_character & 0x3F) | 0x80;
		_character >>= 6;
		buffer[1] = (_character & 0x3F) | 0x80;
		_character >>= 6;
		buffer[0] = _character | 0xF8;

		return std::string(buffer, buffer + 5);
	}
	else if (_character <= 0x7FFFFFFF)
	{
		buffer[5] = (_character & 0x3F) | 0x80;
		_character >>= 6;
		buffer[4] = (_character & 0x3F) | 0x80;
		_character >>= 6;
		buffer[3] = (_character & 0x3F) | 0x80;
		_character >>= 6;
		buffer[2] = (_character & 0x3F) | 0x80;
		_character >>= 6;
		buffer[1] = (_character & 0x3F) | 0x80;
		_character >>= 6;
		buffer[0] = _character | 0xFC;

		return std::string(buffer, buffer + 6);
	}

	// Invalid
	return std::string();
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

	SoundManager* sounds = systemPtr->getSounds();
	if(!sounds->initialize())
	{
		printf("Failed to initialize sound system\n");
		return;
	}
	if(!sounds->loadSounds(systemPtr->getRootDir() / "resources"))
	{
		printf("Failed to load sound resources\n");
		return;
	}

	IInput::ptr input = systemPtr->getInput();

	double previousTime = systemPtr->getTime();
	double currentTime = previousTime;
	double deltaTime = 0.f;

	

	std::cout << "Starting to run" << std::endl;
	//std::cout << "Texture name: \n" << map->getTextureName() << std::endl;

	screenManager.initialize(systemPtr);

	if (!screenManager.openScreen("game"))
	{
		std::cout << "Failed to open screen" << std::endl;
		return;
	}
	
	
	while(!systemPtr->windowIsClosing() && !shouldStop)
	{
		systemPtr->pollEvents();
		
		previousTime = currentTime;
		currentTime = systemPtr->getTime();
		deltaTime = currentTime - previousTime;
		
		std::vector<IInput::Event> events = input->getEvents(true);
				//sounds->playSfx("ball_vs_ball");

		float dt = (float)(deltaTime * 1000.f);
		float bspeed = 0.075f;
		float pspeed = 0.001f;
		

		screenManager.onInput(events);
		screenManager.update((float)deltaTime, graphics);

		const static float FRAMES_PER_SECOND = 60.f;
		const static float FRAME_TIME = 1.f / FRAMES_PER_SECOND;

		//if (deltaTime < FRAME_TIME)
		//{
		//	//std::chrono::milliseconds dura((unsigned int)((FRAME_TIME - deltaTime) * 1000.f));
		//	//std::this_thread::sleep_for(dura);
		//}

		graphics->drawFrame();
		
	}

	sounds->shutdown();
	graphics->destroy();
}
void Game::stop()
{
	shouldStop = true;
}
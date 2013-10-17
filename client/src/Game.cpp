#include "Game.h"

#include "PongOutConfig.h"

#include <Server.h>

//#include <chrono>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <thread>
#include <vector>
#include <chrono>
#include <thread>

bool Game::serverAllow()
{	
	for (int i = 0; i < server->getMsgQueueSize(); i++)
	{
		message tmp = server->getNextMessage();

		switch (tmp.type)
		{
		case msgBase::MsgType::ACKNOWLEDGELAST:
			{
				AcknowledgeLast::ptr rc = boost::static_pointer_cast<AcknowledgeLast>(tmp.msg);
				
				switch(rc->getType())
				{
				case msgBase::MsgType::LOGINREQUEST:
				case msgBase::MsgType::CREATEUSERREQUEST:
					bool allowed = rc->getBool();
					return allowed;
					break;		
				}
				
			}
		/*case msgBase::MsgType::GAMEMESSAGE:
			{
				switch (tmp.gType)
				{
				case GameMessage::GameMsgType::CREATEGAMERESPONSE:
					{

						break;
					}
				}
				break;
			}*/
		}
	}
	return false;
}

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

		server->login(username,password);

		if(!serverAllow())
		{
			screenManager.openScreen("mainmenu");
		}
	}
	else if(_func == "createuser")
	{
		std::string username = screenManager.getText("username");
		std::string password = screenManager.getText("password");

		server->createAccount(username, password);

		if(!serverAllow())
		{
			onFunction("login");
		}
	}
	else if(_func == "host")
	{
		server->createGame( gameSettings.getMapId(), gameSettings.getBallSpeed(), 1, gameSettings.getSuddenDeathTime(), gameSettings.getFOW(), gameSettings.getPOW());
		screenManager.openScreen("game");
	}
	else if(_func.substr(0, 11) == "set/sudden/")
	{
		if(_func.substr(11) == "unlimited")
		{
			gameSettings.setSuddenDeathTime(-1);
		}
		else if(_func.substr(11) == "random")
		{
			int minutes = rand() % 1 + 16;
			gameSettings.setSuddenDeathTime(minutes * 60);
		}
		else
		{
			gameSettings.setSuddenDeathTime(stoi(_func.substr(11)));
		}
	}
	else if(_func.substr(0, 14) == "set/ballspeed/")
	{
		if(_func.substr(14) == "random")
		{
			gameSettings.setBallSpeed((float)rand() / (float)RAND_MAX * 10.f);
		}
		else
		{
			gameSettings.setBallSpeed(stof(_func.substr(14)));
		}
	}
	else if(_func.substr(0, 8) == "set/fow/")
	{
		if(_func.substr(8) == "on")
		{
			gameSettings.setFOW(true);
		}
		else if(_func.substr(8) == "off")
		{
			gameSettings.setFOW(false);
		}
		else if(_func.substr(8) == "random")
		{
			int randomized = rand() % 2;
			
			if(randomized == 1)
			{
				gameSettings.setFOW(true);
			}
			gameSettings.setFOW(false);
		}
	}
	else if(_func.substr(0, 8) == "set/pow/")
	{
		if(_func.substr(8) == "on")
		{
			gameSettings.setPOW(true);
		}
		else if(_func.substr(8) == "off")
		{
			gameSettings.setPOW(false);
		}
		else if(_func.substr(8) == "random")
		{
			int randomized = rand() % 2;

			if(randomized == 1)
			{
				gameSettings.setPOW(true);
			}
			gameSettings.setPOW(false);
		}
	}
	else if(_func.substr(0, 10) == "set/level/")
	{
		gameSettings.setMapId(stoi(_func.substr(12)));
	}
}

Game::Game(ICoreSystem::ptr _system)
	: system(_system),
	screenManager(_system.lock()->getRootDir(), this),
	shouldStop(false)
{
	srand((unsigned int)time((NULL)));
	rand();
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
	server = Server::ptr(new Server("194.47.150.59", 6500));
	server->connect();

	std::cout << "PongOut " << PongOut_VERSION_MAJOR << "." << PongOut_VERSION_MINOR << "." << PongOut_VERSION_PATCH << std::endl;

	std::shared_ptr<ICoreSystem> systemPtr(system);

	IGraphics::ptr graphics = systemPtr->getGraphics();
	if (!graphics->init())
	{
		printf("Failed to initialize renderer\n");
		return;
	}
	graphics->loadResources(systemPtr->getRootDir() / "resources");

	SoundManager *sounds = systemPtr->getSounds();
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

	if (!screenManager.initialize(systemPtr, server))
	{
		std::cout << "Failed to initialize the screen manager." << std::endl;
		return;
	}

	if (!screenManager.openScreen("login"))
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

		screenManager.onInput(events);
		screenManager.update((float)deltaTime, graphics);

		const static float FRAMES_PER_SECOND = 60.f;
		const static float FRAME_TIME = 1.f / FRAMES_PER_SECOND;

		graphics->drawFrame();		
	}

	sounds->shutdown();
	graphics->destroy();
}
void Game::stop()
{
	shouldStop = true;
}
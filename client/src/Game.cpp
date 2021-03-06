#include "Game.h"

#include "PongOutConfig.h"

#include <Server.h>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <chrono>
#include <thread>

void Game::onFunction(const std::string& _func)

{
	std::cout << "Performing: " << _func << std::endl;

	if (_func == "exit")
	{
		stop();
	}
	else if (_func == "login")
	{
		if (state != UserState::CONNECTED)
		{
			if (state == UserState::DISCONNECTED)
			{
				server->connect();
			}
			else
			{
				std::cout << "Wrong state for logging in!" << std::endl;
				return;
			}
		}

		std::string username = screenManager.getText("username");
		std::string password = screenManager.getText("password");

		server->login(username,password);
		state = UserState::WAITING_FOR_LOGIN_RESPONSE;
	}
	else if(_func == "createuser")
	{
//		std::string username = screenManager.getText("username");
//		std::string password = screenManager.getText("password");
//
//		server->createAccount(username, password);
	}
	else if(_func == "host")
	{
		int mapId = gameSettings.getMapId();
		float ballSpeed = gameSettings.getBallSpeed();
		int timeLimit = gameSettings.getSuddenDeathTime();
		bool fow = gameSettings.getFOW();
		bool pow = gameSettings.getPOW();
		server->createGame( mapId, (int)ballSpeed, 1, timeLimit, fow, pow);

		state = UserState::WAITING_FOR_OPPONENT;
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
	else if(_func == "giveUp")
	{
		screenManager.goBack();
		server->giveUpGame();
	}
}

Game::Game(ICoreSystem::ptr _system)
	: system(_system),
	screenManager(_system.lock()->getRootDir(), this),
	shouldStop(false),
	state(UserState::DISCONNECTED)
{
	srand((unsigned int)time((NULL)));
	rand();
}

void Game::run()
{
	server = Server::ptr(new Server("85.24.168.163", 6500));

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

	if (!screenManager.initialize(systemPtr, server))
	{
		std::cout << "Failed to initialize the screen manager." << std::endl;
		return;
	}

	gameState = GameState::ptr(new GameState());

	screenManager.registerScreenState("game", gameState);

	if (!screenManager.openScreen("login"))
	{
		std::cout << "Failed to open screen" << std::endl;
		return;
	}

	while(!systemPtr->windowIsClosing() && !shouldStop)
	{
		if (state != UserState::IN_GAME)
		{
			handleNetworkPackages();
		}
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
	
	screenManager.destroy();
	sounds->shutdown();
	graphics->destroy();
	server->logout();
	server.reset();
}

void Game::stop()
{
	shouldStop = true;
}

void Game::handleNetworkPackages()
{
	while (server->getMsgQueueSize() > 0)
	{
		message msg = server->getNextMessage();

		switch (msg.type)
		{
		case msgBase::MsgType::ACKNOWLEDGELAST:
			{
				AcknowledgeLast::ptr ackMsg = boost::static_pointer_cast<AcknowledgeLast>(msg.msg);
				bool success = !ackMsg->getBool();

				if (state == UserState::WAITING_FOR_LOGIN_RESPONSE)
				{
					if (success)
					{
						state = UserState::AVAILABLE;
						screenManager.openScreen("mainmenu");
					}
				}
				else
				{
					std::cout << "Unknown acknowledge last" << std::endl;
				}
			}
			break;

		case msgBase::MsgType::INTERNALMESSAGE:
			if (msg.strMsg == "Connection established")
			{
				if (state == UserState::DISCONNECTED)
				{
					state = UserState::CONNECTED;
				}
				else if (state != UserState::WAITING_FOR_LOGIN_RESPONSE)
				{
					std::cout << "Error: Received connection success while connected" << std::endl;
				}
			}
			else if (msg.strMsg == "Write successful")
			{
				// Ignore
			}
			else if (msg.strMsg == "disconnected by server")
			{
				state = UserState::DISCONNECTED;
			}
			else
			{
				std::cout << "Unhandled internal message received: " << msg.strMsg << std::endl;
			}

			break;

		case msgBase::MsgType::GAMEMESSAGE:
			if (state != UserState::IN_GAME
				&& !((state == UserState::AVAILABLE || state == UserState::WAITING_FOR_OPPONENT) && msg.gType == GameMessage::GameMsgType::CREATEGAMERESPONSE))
			{
				std::cout << "Received create game response at the wrong time" << std::endl;
				break;
			}

			switch (msg.gType)
			{
			case GameMessage::GameMsgType::CREATEGAMERESPONSE:
				state = UserState::IN_GAME;
								
				if(gameState)
				{
					CreateGameResponse::ptr cgrp = boost::static_pointer_cast<CreateGameResponse>(msg.gMsg);
					if(screenManager.openScreen("game"))
					{
						gameState->load(cgrp);
						gameState->addStateAction(&screenManager);
					}
					else
					{
						std::cout << "Failed to open game state" << std::endl;
					}
				}
				return;

			default:
				if (msg.gMsg)
				{
					std::cout << "Unhandled game package type received: " << (int)msg.gMsg->getGameType() << std::endl;
				}
				else
				{
					std::cout << "Unhandled game package type received" << std::endl;
				}
			}

			break;

		default:
			if (msg.msg)
			{
				std::cout << "Unhandled package type received: " << msg.msg->getType() << std::endl;
			}
			else
			{
				std::cout << "Unhandled package type received" << std::endl;
			}
			break;
		}
	}
}

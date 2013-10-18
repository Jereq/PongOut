#include "stdafx.h"
#include "Referee.h"
#include "TMLReader.h"
#include "CreateGameResponse.h"

#include <chrono>

Referee::Referee(void)
{
}


Referee::~Referee(void)
{
}

void Referee::init( User::ptr _u0, User::ptr _u1, CommonTypes::GameInitInfo _info, int _refID)
{
	user0 = _u0;
	user1 = _u1;
	info = _info;

	user0->setReffereeID(_refID);
	user1->setReffereeID(_refID);

	std::vector<CommonTypes::Block> mapData;

	TMLReader tml;
	tml.readFile("../test.tml");

	TMLNODE r = tml.getRoot();
	TMLNODE mapNode, blockNode;
	for( TMLNODE n : tml.getRoot().nodes )
	{
		if(n.name == "Map")
		{
			for(std::pair<std::string,std::string> m : n.members)
			{
				if(m.first == "id")
				{
					int id = atoi(m.second.c_str());
					if(id == _info.mapID)
					{
						mapNode = n;
					}
				}
			}
		}
		if(n.name == "Blocks")
		{
			blockNode = n;
		}
	}

	int id;
	std::string name;
	std::vector<std::string> rows;
	for(std::pair<std::string,std::string> m : mapNode.members)
	{
		if(m.first == "id")
		{
			id = atoi(m.second.c_str());
		}
		else if(m.first == "name")
		{
			name = m.second;
		}
		else if(m.first == "row")
		{
			rows.push_back(m.second);
		}
	}

	int offX = 0;
	int offY = 0;
	int blockUniqeID = 0;

	for(std::string row : rows )
	{
		for(unsigned int i = 0; i < row.size(); i++)
		{
			std::string val = row.substr(i,1);
			if(val == "0")
				continue;

			CommonTypes::Block bd;
			bd.health = 0;
			std::vector<std::string> textures;
			for(TMLNODE n : blockNode.nodes)
			{
				for(std::pair<std::string,std::string> m : n.members)
				{
					if(m.first == "id")
					{
						std::string id = m.second;
						if(id == val)
						{
							float x,y;
							x = 880.0f / 2.0f + 64.0f * i;
							y = 800.0f / 2.0f + offY * 32.0f;

							bd.x = x;
							bd.y = y;
							bd.z = 0;
							bd.type = stoi(val);
							bd.id = blockUniqeID;
							blockUniqeID++;
						}
					}
					if(m.first == "texture")
					{
						textures.push_back(m.second);
					}
				}
			}
			bd.health = textures.size();
			bd.textures = textures;
			mapData.push_back(bd);
		}

		offY++;
	}	

	user0Info.ball.id = 0;
	user0Info.ball.pos = glm::vec2(0.0f, 0.0f);
	user0Info.ball.vel = glm::vec2(0.0f, 0.0f);

	user0Info.paddle.id = 0;
	user0Info.paddle.pos = glm::vec2(0.0f, 0.0f);
	user0Info.paddle.vel = glm::vec2(0.0f, 0.0f);

	user0Info.score = 0;
	user0Info.userID = user0->getUserID();

	user1Info.ball.id = 1;
	user1Info.ball.pos = glm::vec2(0.0f, 0.0f);
	user1Info.ball.vel = glm::vec2(0.0f, 0.0f);

	user1Info.paddle.id = 1;
	user1Info.paddle.pos = glm::vec2(0.0f, 0.0f);
	user1Info.paddle.vel = glm::vec2(0.0f, 0.0f);

	user1Info.score = 0;
	user1Info.userID = user1->getUserID();

	CreateGameResponse::ptr cgrp0 = CreateGameResponse::ptr(new CreateGameResponse());
	CreateGameResponse::ptr cgrp1 = CreateGameResponse::ptr(new CreateGameResponse());

	cgrp0->setResponse(mapData, _info, user0Info, user1Info);
	cgrp1->setResponse(mapData, _info, user1Info, user0Info);

	user0->addMsgToMsgQueue(cgrp0);
	user1->addMsgToMsgQueue(cgrp1);

	user0->setUserState(User::UserState::INGAME);
	user1->setUserState(User::UserState::INGAME);

	startGame();
}

void Referee::startGame()
{
	gameThread = std::thread(boost::bind(&Referee::gameThreadFunc, shared_from_this()));
}

void Referee::gameThreadFunc()
{
	try
	{
		for (ever) //Tick loop
		{
			while (!refMsgQ.baseQueue.empty())
			{
				auto msg = refMsgQ.pop();

				if (msg.first->getGameType() == GameMessage::GameMsgType::PADDLEUPDATEREQUEST)
				{
					PaddleUpdateRequest::ptr pur = boost::static_pointer_cast<PaddleUpdateRequest>(msg.first);

					if (msg.second == user0->getUserID())
					{
						user0Info.paddle = pur->getPaddle();
					} 
					else if (msg.second == user1->getUserID())
					{
						user1Info.paddle = pur->getPaddle();
					}
					else
					{
						Log::addLog(Log::LogType::LOG_ERROR, 1, "Error in code at:" + std::to_string(__LINE__));
					}
				}
				else
				{
					Log::addLog(Log::LogType::LOG_ERROR, 1, "Error in code at:" + std::to_string(__LINE__));
				}
			}

			//TODO: add ball and block logic!






			GameTickUpdate::ptr gtup0 = GameTickUpdate::ptr(new GameTickUpdate());
			GameTickUpdate::ptr gtup1 = GameTickUpdate::ptr(new GameTickUpdate());

			gtup0->setTickUpdate(user0Info, user1Info, std::vector<CommonTypes::Block>());
			gtup1->setTickUpdate(user1Info, user0Info, std::vector<CommonTypes::Block>());

			user0->addMsgToMsgQueue(gtup0);
			user1->addMsgToMsgQueue(gtup1);
		}
	}
	catch (...)
	{
		Log::addLog(Log::LogType::LOG_ERROR, 1, "Referee rage quit!");
	}
}

void Referee::addMsgToQ( std::pair<GameMessage::ptr, int> _msg )
{
	refMsgQ.push(_msg);
}

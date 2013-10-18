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

							bd.pos.x = x;
							bd.pos.y = y;
							bd.type = stoi(val);
							bd.id = blockUniqeID;
							bd.inPlay = false;
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
	user0Info.ball.radius = 32.0f;
	user0Info.ball.inPlay = true;

	user0Info.paddle.id = 0;
	user0Info.paddle.pos = glm::vec2(0.0f, 0.0f);
	user0Info.paddle.vel = glm::vec2(0.0f, 0.0f);
	user0Info.paddle.inPlay = true;

	user0Info.score = 0;
	user0Info.userID = user0->getUserID();

	user1Info.ball.id = 1;
	user1Info.ball.pos = glm::vec2(0.0f, 0.0f);
	user1Info.ball.vel = glm::vec2(0.0f, 0.0f);
	user1Info.ball.radius = 32.0f;
	user1Info.ball.inPlay = true;

	user1Info.paddle.id = 1;
	user1Info.paddle.pos = glm::vec2(0.0f, 0.0f);
	user1Info.paddle.vel = glm::vec2(0.0f, 0.0f);
	user1Info.paddle.inPlay = true;

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

			bounceOnPaddle(user0Info.ball, user0Info.paddle);
			bounceOnPaddle(user0Info.ball, user1Info.paddle);
			bounceOnPaddle(user1Info.ball, user0Info.paddle);
			bounceOnPaddle(user1Info.ball, user1Info.paddle);

			bounceOnBlock(user0Info);
			bounceOnBlock(user1Info);

			bounceOnPlayArea(user0Info.ball);
			bounceOnPlayArea(user1Info.ball);

			user0Info.ball.pos = user0Info.ball.vel * (float)info.ballSpeed * dt;
			user1Info.ball.pos = user1Info.ball.vel * (float)info.ballSpeed * dt;

			GameTickUpdate::ptr gtup0 = GameTickUpdate::ptr(new GameTickUpdate());
			GameTickUpdate::ptr gtup1 = GameTickUpdate::ptr(new GameTickUpdate());

			gtup0->setTickUpdate(user0Info, user1Info, std::vector<CommonTypes::Block>());
			gtup1->setTickUpdate(user1Info, user0Info, std::vector<CommonTypes::Block>());

			user0->addMsgToMsgQueue(gtup0);
			user1->addMsgToMsgQueue(gtup1);

			std::chrono::milliseconds pause(20);
			std::this_thread::sleep_for(pause);
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

void Referee::bounceOnPlayArea( CommonTypes::Ball& _ball )
{
	glm::vec2 mapSize(1200.0f, 600.0f);
	glm::vec2 clipArea = glm::vec2( mapSize.x / 1280.0f * 2.f, mapSize.y / 720.0f * 2.f);

	glm::vec2 min = glm::vec2(-clipArea.x / 2,-clipArea.y / 2);
	glm::vec2 max = glm::vec2(clipArea.x / 2,clipArea.y / 2);

	if(_ball.pos.x - _ball.radius / 2 < min.x)
	{
		_ball.pos.x = min.x + _ball.radius / 2.0f;
		_ball.vel.x *= -1;
	}
	else if (_ball.pos.x + _ball.radius / 2 > max.x)
	{
		_ball.pos.x = max.x - _ball.radius / 2.0f;
		_ball.vel.x *= -1;
	}
	if(_ball.pos.y - _ball.radius / 2 < min.y)
	{
		_ball.pos.y = min.y + _ball.radius / 2.0f;
		_ball.vel.y *= -1;
		bindInPlayState(_ball);
	}
	else if(_ball.pos.y + _ball.radius / 2 > max.y)
	{
		_ball.pos.y = max.y - _ball.radius / 2.0f;
		_ball.vel.y *= -1;
		bindInPlayState(_ball);
	}
}

void Referee::bounceOnBlock( CommonTypes::PlayerMatchInfo& _pmi )
{
	glm::vec2 center(_pmi.ball.pos.swizzle(glm::X, glm::Y));

	for (CommonTypes::Block b : mapData)
	{
		if ((bool)b.inPlay)
		{
			continue;
		}

		glm::vec2 reflectDir;

		if (ballRectCollide(reflectDir, _pmi.ball, b.pos))
		{
			_pmi.ball.vel = glm::reflect(_pmi.ball.vel, reflectDir);
			_pmi.score++;
		}
	}
}

void Referee::bounceOnPaddle( CommonTypes::Ball& _ball, CommonTypes::Paddle _paddle )
{

	if((bool)_ball.inPlay && (_ball.id == _paddle.id))
	{
		_ball.pos.x = _paddle.pos.x;

		if(_paddle.pos.y > _ball.pos.y)
		{
			_ball.pos.y = _paddle.pos.y - _ball.radius;
		}
		else
		{
			_ball.pos.y = _paddle.pos.y + _ball.radius;
		}
	}

	if(!(bool)_paddle.inPlay && (bool)_ball.inPlay && (_ball.id == _paddle.id))
	{
		_ball.inPlay = false;

		if(_paddle.pos.y > _ball.pos.y)
		{
			_ball.vel.y = -1.f;
		}
		else
		{
			_ball.vel.y = 1.f;
		}

		_ball.vel.x = _paddle.vel.x;
	}

	if(!(bool)_ball.inPlay)
	{
		glm::vec2 reflectDir;
		if (ballRectCollide(reflectDir, _ball, _paddle.pos))
		{
			_ball.vel = glm::normalize(glm::vec2(_ball.pos - _paddle.pos)) * glm::length(_ball.vel);
		}
	}
}

void Referee::bindInPlayState( CommonTypes::Ball& _ball )
{
	_ball.inPlay = true;

	if (_ball.id == user0->getUserID())
	{
		user0Info.paddle.inPlay = true;
	} 
	else if (_ball.id == user1->getUserID())
	{
		user1Info.paddle.inPlay = true;
	}
}

bool Referee::ballRectCollide( glm::vec2& _reflectDir, CommonTypes::Ball& _ball, glm::vec2& _rect )
{
	glm::vec2 center(_ball.pos);

	float rleftX	= _rect.x - blockSize.x * 0.5f;
	float rrightX	= _rect.x + blockSize.x * 0.5f;
	float rTopY		= _rect.y + blockSize.y * 0.5f;
	float rBottomY	= _rect.y - blockSize.y * 0.5f;

	glm::vec2 corners[4] = {
		glm::vec2(rleftX, rTopY),
		glm::vec2(rleftX, rBottomY),
		glm::vec2(rrightX, rBottomY),
		glm::vec2(rrightX, rTopY)
	};

	if (circleLineIntersect(center, _ball.radius, corners[0], corners[1]))
	{
		if (_ball.vel.x > 0.f)
		{
			_reflectDir = glm::vec2(-1.f, 0.f);
			return true;
		}
	}
	else if (circleLineIntersect(center, _ball.radius, corners[1], corners[2]))
	{
		if (_ball.vel.y > 0.f)
		{
			_reflectDir = glm::vec2(0.f, -1.f);
			return true;
		}
	}
	else if (circleLineIntersect(center, _ball.radius, corners[2], corners[3]))
	{
		if (_ball.vel.x < 0.f)
		{
			_reflectDir = glm::vec2(1.f, 0.f);
			return true;
		}
	}
	else if (circleLineIntersect(center, _ball.radius, corners[3], corners[0]))
	{
		if (_ball.vel.y < 0.f)
		{
			_reflectDir = glm::vec2(0.f, 1.f);
			return true;
		}
	}
	else
	{
		for (glm::vec2& corner : corners)
		{
			if (circleCirleIntersect(center, _ball.radius, corner, 0.f))
			{
				if (glm::dot(_ball.vel, corner - center) >= 0.f)
				{
					_reflectDir = glm::normalize(center - corner);
					return true;
				}
			}
		}
	}

	return false;
}

bool Referee::circleLineIntersect( glm::vec2 _circleCenter, float _circleRadius, glm::vec2 _lineStart, glm::vec2 _lineEnd )
{
	glm::vec2 circleRelPos = _circleCenter - _lineStart;
	glm::vec2 lineRelEnd = _lineEnd - _lineStart;

	if (lineRelEnd == glm::vec2(0.f, 0.f))
	{
		return false;
	}

	glm::vec2 lineDir = glm::normalize(lineRelEnd);
	float circleLengthAlong = glm::dot(circleRelPos, lineDir);

	if (circleLengthAlong < 0.f || circleLengthAlong * circleLengthAlong > glm::dot(lineRelEnd, lineRelEnd))
	{
		return false;
	}

	glm::vec2 normDistance = circleRelPos - lineDir * circleLengthAlong;
	float normLengthSqr = glm::dot(normDistance, normDistance);

	if (normLengthSqr > _circleRadius * _circleRadius)
	{
		return false;
	}

	return true;
}

bool Referee::circleCirleIntersect( glm::vec2 _center1, float _radius1, glm::vec2 _center2, float _radius2 )
{
	glm::vec2 relPos = _center1 - _center2;
	float radiusSum = _radius1 + _radius2;
	return glm::dot(relPos, relPos) <= radiusSum * radiusSum;
}

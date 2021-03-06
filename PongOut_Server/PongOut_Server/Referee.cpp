#include "stdafx.h"
#include "Referee.h"
#include "TMLReader.h"
#include "CreateGameResponse.h"
#include "GameMaster.h"
#include "EndGameResponse.h"
#include "EndGameRequest.h"

#include <chrono>
#include <thread>

const glm::vec2 Referee::mapSize(1200.0f, 600.0f);

Referee::Referee(void)
{
}


Referee::~Referee(void)
{
	if (gameThread.get_id() == std::this_thread::get_id())
	{
		gameThread.detach();
	}
	else if (gameThread.joinable())
	{
		gameThread.join();
	}
}

void Referee::init( User::ptr _u0, User::ptr _u1, CommonTypes::GameInitInfo _info, int _refID)
{
	user0 = _u0;
	user1 = _u1;
	info = _info;
	refID = _refID;

	user0->setReffereeID(_refID);
	user1->setReffereeID(_refID);

#pragma region TrollML

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

	const static glm::vec2 BLOCK_SIZE(64.f, 32.f);

	for(std::string row : rows )
	{
		for(unsigned int i = 0; i < row.size(); i++)
		{
			std::string val = row.substr(i,1);
			if(val == "0")
				continue;

			float startPosX = mapSize.x / 2.0f - row.length() / 2.0f * BLOCK_SIZE.x + BLOCK_SIZE.x / 2.0f;
			float startPosY = mapSize.y / 2.0f - rows.size() / 2.0f * BLOCK_SIZE.y + BLOCK_SIZE.y / 2.0f;

			CommonTypes::Block bd;
			bd.health = 0;
			std::vector<std::string> textures;
			std::string id = "0";
			for(TMLNODE n : blockNode.nodes)
			{
				for(std::pair<std::string,std::string> m : n.members)
				{
					//std::string id;
					if(m.first == "id")
					{
						id = m.second;
						if(id == val)
						{
							bd.pos.x = startPosX + BLOCK_SIZE.x * i;
							bd.pos.y = startPosY + BLOCK_SIZE.y * offY;
							bd.type = stoi(val);
							bd.id = blockUniqeID;
							blockUniqeID++;
						}
					}
					if(m.first == "texture" && id == val)
					{
						textures.push_back(m.second);
					}
				}

			}
			bd.health = textures.size();
			bd.textures = textures;
			mapData.push_back(bd);

			//Log::addLog(Log::LogType::LOG_INFO,4, std::to_string(bd.id), __FILE__, __LINE__);
			//for(std::string t : textures)
			//{
			//	Log::addLog(Log::LogType::LOG_INFO,4,t, __FILE__,__LINE__);
			//}

		}

		offY++;
	}	

#pragma endregion TrollML

	const static float BALL_RADIUS = 16.f;
	const static float PADDLE_HEIGHT = 32.f;

	user0Info.paddle.id = user0->getUserID();
	user0Info.paddle.pos = glm::vec2(mapSize.x / 2.f, PADDLE_HEIGHT / 2.f);
	user0Info.paddle.vel = glm::vec2(0.0f, 0.0f);

	user0Info.ball.id = user0->getUserID();
	user0Info.ball.pos = user0Info.paddle.pos + glm::vec2(0.0f, PADDLE_HEIGHT / 2.f + BALL_RADIUS);
	user0Info.ball.vel = glm::vec2(0.0f, 0.0f);
	user0Info.ball.radius = BALL_RADIUS;

	user0Info.score = 0;
	user0Info.userID = user0->getUserID();

	user0HoldingBall = true;
	user0LaunchingBall = false;

	user1Info.paddle.id = user1->getUserID();
	user1Info.paddle.pos = glm::vec2(mapSize.x / 2.f, mapSize.y - PADDLE_HEIGHT / 2.f);
	user1Info.paddle.vel = glm::vec2(0.0f, 0.0f);

	user1Info.ball.id = user1->getUserID();
	user1Info.ball.pos = user1Info.paddle.pos - glm::vec2(0.0f, PADDLE_HEIGHT / 2.f + BALL_RADIUS);
	user1Info.ball.vel = glm::vec2(0.0f, 0.0f);
	user1Info.ball.radius = BALL_RADIUS;

	user1Info.score = 0;
	user1Info.userID = user1->getUserID();

	user1HoldingBall = true;
	user1LaunchingBall = false;

	CreateGameResponse::ptr cgrp0 = CreateGameResponse::ptr(new CreateGameResponse());
	CreateGameResponse::ptr cgrp1 = CreateGameResponse::ptr(new CreateGameResponse());

	cgrp0->setResponse(mapData, _info, user0Info, user1Info);
	cgrp1->setResponse(mapData, _info, user1Info, user0Info);

	user0->addMsgToMsgQueue(cgrp0);
	user1->addMsgToMsgQueue(cgrp1);

	user0->setUserState(User::UserState::INGAME);
	user1->setUserState(User::UserState::INGAME);

	suddenDeathTime = info.suddenDeathTime;

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
						user0Info.paddle.pos = pur->getPaddle().pos;
						user0Info.paddle.vel = pur->getPaddle().vel;
					}
					else if (msg.second == user1->getUserID())
					{
						user1Info.paddle.pos = pur->getPaddle().pos;
						user1Info.paddle.vel = pur->getPaddle().vel;
					}
					else
					{
						Log::addLog(Log::LogType::LOG_ERROR, 1, "Referee gale loop", __FILE__, __LINE__);
					}
				}
				else if (msg.first->getGameType() == GameMessage::GameMsgType::LAUNCH_BALL_REQUEST)
				{
					if (msg.second == user0->getUserID() && user0HoldingBall)
					{
						user0LaunchingBall = true;
					}
					else if (msg.second == user1->getUserID() && user1HoldingBall)
					{
						user1LaunchingBall = true;
					}
				}
				else
				{
					Log::addLog(Log::LogType::LOG_ERROR, 1, "Referee gale loop", __FILE__, __LINE__);
				}
			}

			bounceOnPaddle(user0Info.ball, user0Info.paddle, user0HoldingBall);
			bounceOnPaddle(user0Info.ball, user1Info.paddle, false);
			bounceOnPaddle(user1Info.ball, user0Info.paddle, false);
			bounceOnPaddle(user1Info.ball, user1Info.paddle, user1HoldingBall);

			bounceOnBlock(user0Info);
			bounceOnBlock(user1Info);

			bounceOnPlayArea(user0Info.ball);
			bounceOnPlayArea(user1Info.ball);

			const static float START_VEL = 300.f;

			if (user0LaunchingBall)
			{
				user0Info.ball.vel = glm::normalize(glm::vec2(user0Info.paddle.vel.x, 100.f)) * START_VEL;

				user0HoldingBall = false;
				user0LaunchingBall = false;
			}

			if (user1LaunchingBall)
			{
				user1Info.ball.vel = glm::normalize(glm::vec2(user1Info.paddle.vel.x, -100.f)) * START_VEL;

				user1HoldingBall = false;
				user1LaunchingBall = false;
			}

			user0Info.ball.pos += user0Info.ball.vel * (float)info.ballSpeed * dt;
			user1Info.ball.pos += user1Info.ball.vel * (float)info.ballSpeed * dt;

			GameTickUpdate::ptr gtup0 = GameTickUpdate::ptr(new GameTickUpdate());
			GameTickUpdate::ptr gtup1 = GameTickUpdate::ptr(new GameTickUpdate());

			second += dt;

			if (second >= 1.0f)
			{
				suddenDeathTime--;
				second = 0.0f;
			}

			gtup0->setTickUpdate(user0Info, user1Info, mapData, suddenDeathTime);
			gtup1->setTickUpdate(user1Info, user0Info, mapData, suddenDeathTime);

			if (!user1->getSocket()->is_open())
			{
				EndGameResponse::ptr egrp = EndGameResponse::ptr(new EndGameResponse());

				CommonTypes::MatchResult mr;
				mr.loserScore = user1Info.score;
				mr.winnerScore = user0Info.score;
				mr.winnerID = user0->getUserID();

				egrp->setResult(mr);
				user0->addMsgToMsgQueue(egrp);
				user0->setUserState(User::UserState::AVAILABLE);

				GameMaster::getInstance().removeReferee(refID);

				break;
			}

			if (!user0->getSocket()->is_open())
			{
				EndGameResponse::ptr egrp = EndGameResponse::ptr(new EndGameResponse());

				CommonTypes::MatchResult mr;
				mr.loserScore = user0Info.score;
				mr.winnerScore = user1Info.score;
				mr.winnerID = user1->getUserID();

				egrp->setResult(mr);
				user1->addMsgToMsgQueue(egrp);
				user1->setUserState(User::UserState::AVAILABLE);

				GameMaster::getInstance().removeReferee(refID);

				break;
			}

			user0->addMsgToMsgQueue(gtup0);
			user1->addMsgToMsgQueue(gtup1);

			std::chrono::milliseconds pause(20);
			std::this_thread::sleep_for(pause);
		}
	}
	catch (...)
	{
		Log::addLog(Log::LogType::LOG_ERROR, 1, "Referee rage quit!", __FILE__, __LINE__);
	}
}

void Referee::addMsgToQ( std::pair<GameMessage::ptr, int> _msg )
{
	refMsgQ.push(_msg);
}

void Referee::bounceOnPlayArea( CommonTypes::Ball& _ball )
{
	glm::vec2 min = glm::vec2(0,0);
	glm::vec2 max = mapSize;

	if(_ball.pos.x - _ball.radius < min.x)
	{
		_ball.pos.x = min.x + _ball.radius;
		_ball.vel.x *= -1;
	}
	else if (_ball.pos.x + _ball.radius > max.x)
	{
		_ball.pos.x = max.x - _ball.radius;
		_ball.vel.x *= -1;
	}
	if(_ball.pos.y - _ball.radius < min.y)
	{
		_ball.pos.y = min.y + _ball.radius;
		_ball.vel.y *= -1;
		bindInPlayState(_ball);
		user1Info.score += 10;
	}
	else if(_ball.pos.y + _ball.radius > max.y)
	{
		_ball.pos.y = max.y - _ball.radius;
		_ball.vel.y *= -1;
		bindInPlayState(_ball);
		user0Info.score += 10;
	}
}

void Referee::bounceOnBlock( CommonTypes::PlayerMatchInfo& _pmi )
{
	glm::vec2 center(_pmi.ball.pos.swizzle(glm::X, glm::Y));

	for (CommonTypes::Block& b : mapData)
	{
		if (b.health <= 0)
		{
			continue;
		}

		glm::vec2 reflectDir;

		if (ballRectCollide(reflectDir, _pmi.ball, b.pos, blockSize))
		{
			_pmi.ball.vel = glm::reflect(_pmi.ball.vel, reflectDir);
			_pmi.score++;
			b.health--;
		}
	}
}

void Referee::bounceOnPaddle( CommonTypes::Ball& _ball, CommonTypes::Paddle _paddle, bool _heldOnPaddle)
{
	if(_heldOnPaddle && (_ball.id == _paddle.id))
	{
		_ball.pos.x = _paddle.pos.x;

		if(_paddle.pos.y > _ball.pos.y)
		{
			_ball.pos.y = _paddle.pos.y - _ball.radius - paddleSize.y / 2.f;
		}
		else
		{
			_ball.pos.y = _paddle.pos.y + _ball.radius + paddleSize.y / 2.f;
		}

		_ball.vel = _paddle.vel;
	}
	else
	{
		glm::vec2 reflectDir;
		if (ballRectCollide(reflectDir, _ball, _paddle.pos, paddleSize))
		{
			_ball.vel = glm::normalize(glm::vec2(_ball.pos - _paddle.pos)) * glm::length(_ball.vel);
		}
	}
}

void Referee::bindInPlayState( CommonTypes::Ball& _ball )
{
	if (_ball.id == user0->getUserID())
	{
		user0HoldingBall = true;
	} 
	else if (_ball.id == user1->getUserID())
	{
		user1HoldingBall = true;
	}
}

bool Referee::ballRectCollide( glm::vec2& _reflectDir, CommonTypes::Ball& _ball, glm::vec2& _rectPos, glm::vec2 _rectSize )
{
	glm::vec2 center(_ball.pos);

	float rleftX	= _rectPos.x - _rectSize.x * 0.5f;
	float rrightX	= _rectPos.x + _rectSize.x * 0.5f;
	float rTopY		= _rectPos.y + _rectSize.y * 0.5f;
	float rBottomY	= _rectPos.y - _rectSize.y * 0.5f;

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

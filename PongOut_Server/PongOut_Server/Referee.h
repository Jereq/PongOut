#pragma once

#include "User.h"
#include "CreateGameRequest.h"
#include "PaddleUpdateRequest.h"
#include "GameTickUpdate.h"

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <thread>
#include <SafeQueue.h>

static const glm::vec2 blockSize(64.0f, 32.0f);
static const glm::vec2 paddleSize(128.0f, 32.0f);
static const float dt(0.05f);

class GameMaster;

class Referee : public boost::enable_shared_from_this<Referee>
{
public:

	typedef boost::shared_ptr<Referee> ptr;

	Referee(void);
	~Referee(void);

	void init(User::ptr _u0, User::ptr _u1, CommonTypes::GameInitInfo _info, int _refID);
	void addMsgToQ(std::pair<GameMessage::ptr, int> _msg);

private:

	void startGame();
	void gameThreadFunc();

	void bounceOnPlayArea(CommonTypes::Ball& _ball);
	void bounceOnBlock(CommonTypes::PlayerMatchInfo& _pmi);
	void bounceOnPaddle(CommonTypes::Ball& _ball, CommonTypes::Paddle _paddle, bool _heldOnPaddle);
	void bindInPlayState(CommonTypes::Ball& _ball);
	bool ballRectCollide(glm::vec2& _reflectDir, CommonTypes::Ball& _ball, glm::vec2& _rectPos, glm::vec2 _rectSize );
	bool circleLineIntersect(glm::vec2 _circleCenter, float _circleRadius, glm::vec2 _lineStart, glm::vec2 _lineEnd);
	bool circleCirleIntersect(glm::vec2 _center1, float _radius1, glm::vec2 _center2, float _radius2);

	User::ptr user0, user1;

	const static glm::vec2 mapSize;

	CommonTypes::GameInitInfo info;
	CommonTypes::PlayerMatchInfo user0Info, user1Info;
	bool user0LaunchingBall, user1LaunchingBall;
	bool user0HoldingBall, user1HoldingBall;
	int refID, suddenDeathTime;
	float second;
	std::thread gameThread;
	SafeQueue<std::pair<GameMessage::ptr, int>> refMsgQ;
	std::vector<CommonTypes::Block> mapData;

	
};


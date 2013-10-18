#pragma once

#include "User.h"
#include "CreateGameRequest.h"
#include "PaddleUpdateRequest.h"
#include "GameTickUpdate.h"

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <thread>
#include <SafeQueue.h>

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

	User::ptr user0, user1;
	CommonTypes::GameInitInfo info;
	CommonTypes::PlayerMatchInfo user0Info, user1Info;
	int id;
	std::thread gameThread;
	SafeQueue<std::pair<GameMessage::ptr, int>> refMsgQ;
};


#pragma once
#include "GameMessage.h"

class LaunchBallRequest :
	public GameMessage
{
public:

	typedef boost::shared_ptr<LaunchBallRequest> ptr;

	LaunchBallRequest(void);
	virtual ~LaunchBallRequest(void);

	virtual std::vector<char> getData() override;
	virtual msgBase::ptr interpretPacket(const std::deque<char>& _buffer) override;
};


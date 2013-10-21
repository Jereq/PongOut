#pragma once

#include "GameMessage.h"

class EndGameRequest :	public GameMessage
{
public:

	typedef boost::shared_ptr<EndGameRequest> ptr;

	EndGameRequest(void);
	virtual ~EndGameRequest(void);

	virtual std::vector<char> getData() override;
	virtual msgBase::ptr interpretPacket(const std::deque<char>& _buffer) override;
};


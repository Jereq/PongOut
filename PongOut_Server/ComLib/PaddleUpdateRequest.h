#pragma once

#include "GameMessage.h"

class PaddleUpdateRequest :	public GameMessage
{
public:

	typedef boost::shared_ptr<PaddleUpdateRequest> ptr;

	PaddleUpdateRequest(void);
	virtual ~PaddleUpdateRequest(void);

	virtual std::vector<char> getData() override;
	virtual msgBase::ptr interpretPacket(const std::deque<char>& _buffer) override;

	void setPaddle(const CommonTypes::Paddle& _p);
	CommonTypes::Paddle getPaddle();

private:

	CommonTypes::Paddle p;

};


#pragma once

#include "msgBase.h"

class LogoutRequest : public msgBase
{
public:

	typedef boost::shared_ptr<LogoutRequest> ptr;

	LogoutRequest(void);
	virtual ~LogoutRequest(void);

	virtual std::vector<char> getData() override;
	virtual msgBase::ptr interpretPacket(const std::deque<char>& _buffer) override;

};


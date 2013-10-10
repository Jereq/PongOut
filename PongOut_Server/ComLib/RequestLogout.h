#pragma once

#include "msgbase.h"

class RequestLogout : public msgBase
{
public:

	typedef boost::shared_ptr<RequestLogout> ptr;

	RequestLogout(void);
	virtual ~RequestLogout(void);

	virtual std::vector<char> getData() override;
	virtual msgBase::ptr interpretPacket(const std::deque<char>& _buffer) override;

};


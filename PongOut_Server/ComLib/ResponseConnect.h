#pragma once
#include "msgbase.h"
class ResponseConnect :	public msgBase
{
public:

	typedef boost::shared_ptr<ResponseConnect> ptr;

	ResponseConnect(void);
	virtual ~ResponseConnect(void);

	virtual std::vector<char> getData() override;
	virtual msgBase::ptr interpretPacket(const std::deque<char>& _buffer) override;

private:
};


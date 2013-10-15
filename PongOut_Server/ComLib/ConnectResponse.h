#pragma once
#include "msgbase.h"
class ConnectResponse :	public msgBase
{
public:

	typedef boost::shared_ptr<ConnectResponse> ptr;

	ConnectResponse(void);
	virtual ~ConnectResponse(void);

	virtual std::vector<char> getData() override;
	virtual msgBase::ptr interpretPacket(const std::deque<char>& _buffer) override;

private:
};


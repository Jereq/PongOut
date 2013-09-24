#pragma once
#include "msgbase.h"
#include <utility>

class RequestFriendlist :	public msgBase
{
public:

	typedef boost::shared_ptr<RequestFriendlist> ptr;

	RequestFriendlist(void);
	virtual ~RequestFriendlist(void);

	virtual std::vector<char> getData() override;
	virtual msgBase::ptr interpretPacket(const std::deque<char>& _buffer) override;
};


#pragma once
#include "msgBase.h"
#include <utility>

class FriendlistRequest :	public msgBase
{
public:

	typedef boost::shared_ptr<FriendlistRequest> ptr;

	FriendlistRequest(void);
	virtual ~FriendlistRequest(void);

	virtual std::vector<char> getData() override;
	virtual msgBase::ptr interpretPacket(const std::deque<char>& _buffer) override;
};


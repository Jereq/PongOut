#pragma once
#include "msgBase.h"
#include <utility>

class FriendlistResponse : public msgBase
{
public:

	typedef boost::shared_ptr<FriendlistResponse> ptr;

	FriendlistResponse(void);
	virtual ~FriendlistResponse(void);

	void addToFriendList(const std::pair<std::string, unsigned int>& _friend);

	virtual std::vector<char> getData() override;
	virtual msgBase::ptr interpretPacket(const std::deque<char>& _buffer) override;

	std::vector<std::pair<std::string, unsigned int>> getAllFriends();

private:

	std::vector<std::pair<std::string, unsigned int>> friends;
	
};


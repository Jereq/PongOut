#pragma once
#include "msgbase.h"
#include <utility>

class ResponseFriendlist : public msgBase
{
public:

	typedef boost::shared_ptr<ResponseFriendlist> ptr;

	ResponseFriendlist(void);
	virtual ~ResponseFriendlist(void);

	void addToFriendList(const std::pair<std::string, unsigned int>& _friend);

	virtual std::vector<char> getData() override;
	virtual msgBase::ptr interpretPacket(const std::deque<char>& _buffer) override;

	std::vector<std::pair<std::string, unsigned int>> getAllFriends();

private:

	std::vector<std::pair<std::string, unsigned int>> friends;
	
};


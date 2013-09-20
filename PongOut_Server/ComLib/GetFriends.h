#pragma once
#include "msgbase.h"
class GetFriends : public msgBase
{
public:

	typedef boost::shared_ptr<GetFriends> ptr;

	GetFriends(void);
	virtual ~GetFriends(void);

	void addToFriendList(std::string _userName, boost::uuids::uuid _uuid);

	virtual std::vector<char> getData() override;
	virtual msgBase::ptr interpretPacket(const std::deque<char>& _buffer) override;

	std::vector<userData> getAllFriends();

private:

	std::vector<userData> friends;
	
};


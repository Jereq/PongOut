#include "stdafx.h"
#include "FriendlistResponse.h"


FriendlistResponse::FriendlistResponse(void) : msgBase(msgBase::MsgType::FRIENDLISTRESPONSE)
{
}


FriendlistResponse::~FriendlistResponse(void)
{
}

void FriendlistResponse::addToFriendList(const std::pair<std::string, unsigned int>& _friend)
{
	friends.push_back(_friend);
}

std::vector<char> FriendlistResponse::getData()
{
	std::vector<char> res;
	std::back_insert_iterator<std::vector<char>> iter(res);
	pack(msgHeader, iter);
	pack(friends, iter);

	msgHeader.length = res.size() - sizeof(msgBase::header);
	pack(msgHeader, res.begin());
	return res;
}

msgBase::ptr FriendlistResponse::interpretPacket( const std::deque<char>& _buffer )
{
	FriendlistResponse::ptr gfp = FriendlistResponse::ptr(new FriendlistResponse());
	std::deque<char>::const_iterator it = _buffer.cbegin();
	it = unpack(gfp->msgHeader, it);
	it = unpack(gfp->friends, it);

	return gfp;
}

std::vector<std::pair<std::string, unsigned int>> FriendlistResponse::getAllFriends()
{
	return friends;
}

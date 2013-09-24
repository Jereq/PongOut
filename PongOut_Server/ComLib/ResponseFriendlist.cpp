#include "stdafx.h"
#include "ResponseFriendlist.h"


ResponseFriendlist::ResponseFriendlist(void) : msgBase(msgBase::MsgType::RESPONSEFRIENDLIST)
{
}


ResponseFriendlist::~ResponseFriendlist(void)
{
}

void ResponseFriendlist::addToFriendList( const std::pair<std::string, boost::uuids::uuid>& _friend)
{
	friends.push_back(_friend);
}

std::vector<char> ResponseFriendlist::getData()
{
	std::vector<char> res;
	std::back_insert_iterator<std::vector<char>> iter(res);
	pack(msgHeader, iter);
	pack(friends, iter);

	msgHeader.length = res.size() - sizeof(msgBase::header);
	pack(msgHeader, res.begin());
	return res;
}

msgBase::ptr ResponseFriendlist::interpretPacket( const std::deque<char>& _buffer )
{
	ResponseFriendlist::ptr gfp = ResponseFriendlist::ptr(new ResponseFriendlist());
	std::deque<char>::const_iterator it = _buffer.cbegin();
	it = unpack(gfp->msgHeader, it);
	it = unpack(gfp->friends, it);

	return gfp;
}

std::vector<std::pair<std::string, boost::uuids::uuid>> ResponseFriendlist::getAllFriends()
{
	return friends;
}

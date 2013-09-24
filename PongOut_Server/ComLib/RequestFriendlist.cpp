#include "stdafx.h"
#include "RequestFriendlist.h"


RequestFriendlist::RequestFriendlist(void) : msgBase(msgBase::MsgType::REQUESTFRIENDLIST)
{
	msgHeader.length = 0;
}


RequestFriendlist::~RequestFriendlist(void)
{
}

std::vector<char> RequestFriendlist::getData()
{
	std::vector<char> res;
	std::back_insert_iterator<std::vector<char>> iter(res);
	pack(msgHeader, iter);

	return res;
}

msgBase::ptr RequestFriendlist::interpretPacket( const std::deque<char>& _buffer )
{
	RequestFriendlist::ptr sfp = RequestFriendlist::ptr(new RequestFriendlist());
	std::deque<char>::const_iterator it = _buffer.cbegin();
	it = unpack(sfp->msgHeader, it);

	return sfp;
}

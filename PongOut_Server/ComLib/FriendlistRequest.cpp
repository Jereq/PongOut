#include "stdafx.h"
#include "FriendlistRequest.h"


FriendlistRequest::FriendlistRequest(void) : msgBase(msgBase::MsgType::FRIENDLISTREQUEST)
{
	msgHeader.length = 0;
}


FriendlistRequest::~FriendlistRequest(void)
{
}

std::vector<char> FriendlistRequest::getData()
{
	std::vector<char> res;
	std::back_insert_iterator<std::vector<char>> iter(res);
	pack(msgHeader, iter);

	return res;
}

msgBase::ptr FriendlistRequest::interpretPacket( const std::deque<char>& _buffer )
{
	FriendlistRequest::ptr sfp = FriendlistRequest::ptr(new FriendlistRequest());
	std::deque<char>::const_iterator it = _buffer.cbegin();
	it = unpack(sfp->msgHeader, it);

	return sfp;
}

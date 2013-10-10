#include "stdafx.h"
#include "RequestLogout.h"


RequestLogout::RequestLogout(void) : msgBase(msgBase::MsgType::REQUESTLOGOUT)
{
}


RequestLogout::~RequestLogout(void)
{
}

std::vector<char> RequestLogout::getData()
{
	msgHeader.length = 0;
	std::vector<char> res;
	std::back_insert_iterator<std::vector<char>> iter(res);
	pack(msgHeader, iter);
	
	return res;
}

msgBase::ptr RequestLogout::interpretPacket( const std::deque<char>& _buffer )
{
	RequestLogout::ptr rlop = RequestLogout::ptr(new RequestLogout());
	std::deque<char>::const_iterator it = _buffer.cbegin();
	it = unpack(rlop->msgHeader, it);
	return rlop;
}

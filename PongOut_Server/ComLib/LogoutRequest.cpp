#include "stdafx.h"
#include "LogoutRequest.h"


LogoutRequest::LogoutRequest(void) : msgBase(msgBase::MsgType::LOGOUTREQUEST)
{
}


LogoutRequest::~LogoutRequest(void)
{
}

std::vector<char> LogoutRequest::getData()
{
	msgHeader.length = 0;
	std::vector<char> res;
	std::back_insert_iterator<std::vector<char>> iter(res);
	pack(msgHeader, iter);
	
	return res;
}

msgBase::ptr LogoutRequest::interpretPacket( const std::deque<char>& _buffer )
{
	LogoutRequest::ptr rlop = LogoutRequest::ptr(new LogoutRequest());
	std::deque<char>::const_iterator it = _buffer.cbegin();
	it = unpack(rlop->msgHeader, it);
	return rlop;
}

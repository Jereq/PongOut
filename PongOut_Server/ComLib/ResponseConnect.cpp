#include "stdafx.h"
#include "ResponseConnect.h"


ResponseConnect::ResponseConnect(void) : msgBase(msgBase::MsgType::RESPONSECONNECT)
{
	msgHeader.length = 0;
}


ResponseConnect::~ResponseConnect(void)
{
}

std::vector<char> ResponseConnect::getData()
{
	std::vector<char> res;
	std::back_insert_iterator<std::vector<char>> iter(res);
	pack(msgHeader, iter);

	return res;
}

msgBase::ptr ResponseConnect::interpretPacket( const std::deque<char>& _buffer )
{
	ResponseConnect::ptr rc = ResponseConnect::ptr(new ResponseConnect());
	std::deque<char>::const_iterator it = _buffer.cbegin();
	it = unpack(rc->msgHeader, it);

	return rc;
}

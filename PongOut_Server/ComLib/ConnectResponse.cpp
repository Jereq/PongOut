#include "stdafx.h"
#include "ConnectResponse.h"


ConnectResponse::ConnectResponse(void) : msgBase(msgBase::MsgType::CONNECTRESPONSE)
{
	msgHeader.length = 0;
}


ConnectResponse::~ConnectResponse(void)
{
}

std::vector<char> ConnectResponse::getData()
{
	std::vector<char> res;
	std::back_insert_iterator<std::vector<char>> iter(res);
	pack(msgHeader, iter);

	return res;
}

msgBase::ptr ConnectResponse::interpretPacket( const std::deque<char>& _buffer )
{
	ConnectResponse::ptr rc = ConnectResponse::ptr(new ConnectResponse());
	std::deque<char>::const_iterator it = _buffer.cbegin();
	it = unpack(rc->msgHeader, it);

	return rc;
}

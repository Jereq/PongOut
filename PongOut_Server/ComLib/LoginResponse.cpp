#include "stdafx.h"
#include "LoginResponse.h"


LoginResponse::LoginResponse(void) : msgBase(msgBase::MsgType::LOGINRESPONSE)
{
}


LoginResponse::~LoginResponse(void)
{
}

void LoginResponse::setLoginFailure( bool _failure )
{
	loginFailure = _failure;

	msgHeader.length = sizeof(char);
}

std::vector<char> LoginResponse::getData()
{
	std::vector<char> res;
	std::back_insert_iterator<std::vector<char>> iter(res);
	pack(msgHeader, iter);
	pack(loginFailure, iter);

	return res;
}

msgBase::ptr LoginResponse::interpretPacket( const std::deque<char>& _buffer )
{
	LoginResponse::ptr rl = LoginResponse::ptr(new LoginResponse());
	std::deque<char>::const_iterator it = _buffer.cbegin();
	it = unpack(rl->msgHeader, it);
	it = unpack(rl->loginFailure, it);

	return rl;
}

bool LoginResponse::getLoginFailure()
{
	return loginFailure;
}

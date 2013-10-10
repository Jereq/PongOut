#include "stdafx.h"
#include "ResponseLogin.h"


ResponseLogin::ResponseLogin(void) : msgBase(msgBase::MsgType::RESPONSELOGIN)
{
}


ResponseLogin::~ResponseLogin(void)
{
}

void ResponseLogin::setLoginFailure( bool _failure )
{
	loginFailure = _failure;

	msgHeader.length = sizeof(char);
}

std::vector<char> ResponseLogin::getData()
{
	std::vector<char> res;
	std::back_insert_iterator<std::vector<char>> iter(res);
	pack(msgHeader, iter);
	pack(loginFailure, iter);

	return res;
}

msgBase::ptr ResponseLogin::interpretPacket( const std::deque<char>& _buffer )
{
	ResponseLogin::ptr rl = ResponseLogin::ptr(new ResponseLogin());
	std::deque<char>::const_iterator it = _buffer.cbegin();
	it = unpack(rl->msgHeader, it);
	it = unpack(rl->loginFailure, it);

	return rl;
}

bool ResponseLogin::getLoginFailure()
{
	return loginFailure;
}

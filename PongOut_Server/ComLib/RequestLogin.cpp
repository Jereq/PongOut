#include "stdafx.h"
#include "RequestLogin.h"


RequestLogin::RequestLogin(void) : msgBase(msgBase::MsgType::REQUESTLOGIN)
{
}


RequestLogin::~RequestLogin(void)
{
}

std::vector<char> RequestLogin::getData()
{
	std::vector<char> res;
	std::back_insert_iterator<std::vector<char>> it(res);
	pack(msgHeader, it);
	pack(userName, it);
	pack(password, it);
	return res;
}

msgBase::ptr RequestLogin::interpretPacket( const std::deque<char>& _buffer )
{
	RequestLogin::ptr lp = RequestLogin::ptr(new RequestLogin());
	std::deque<char>::const_iterator it = _buffer.cbegin();
	it = unpack(lp->msgHeader, it);
	it = unpack(lp->userName, it);
	it = unpack(lp->password, it);
	return lp;
}

void RequestLogin::setLogin( const std::string& _userName, const std::string& _password )
{
	userName = _userName;
	password = encryptPassword(_password);
	msgHeader.length = userName.length() + password.length() + sizeof(std::uint16_t) * 2;
}

std::string RequestLogin::encryptPassword( std::string _password )
{
	//TODO: encrypt password!!
	return _password;
}

std::string RequestLogin::getUsername()
{
	return userName;
}

std::string RequestLogin::getPassword()
{
	return password;
}

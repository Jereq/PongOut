#include "stdafx.h"
#include "LoginRequest.h"


LoginRequest::LoginRequest(void) : msgBase(msgBase::MsgType::LOGINREQUEST)
{
}


LoginRequest::~LoginRequest(void)
{
}

std::vector<char> LoginRequest::getData()
{
	std::vector<char> res;
	std::back_insert_iterator<std::vector<char>> it(res);
	pack(msgHeader, it);
	pack(userName, it);
	pack(password, it);
	return res;
}

msgBase::ptr LoginRequest::interpretPacket( const std::deque<char>& _buffer )
{
	LoginRequest::ptr lp = LoginRequest::ptr(new LoginRequest());
	std::deque<char>::const_iterator it = _buffer.cbegin();
	it = unpack(lp->msgHeader, it);
	it = unpack(lp->userName, it);
	it = unpack(lp->password, it);
	return lp;
}

void LoginRequest::setLogin( const std::string& _userName, const std::string& _password )
{
	userName = _userName;
	password = encryptPassword(_password);
	msgHeader.length = userName.length() + password.length() + sizeof(std::uint16_t) * 2;
}

std::string LoginRequest::encryptPassword( std::string _password )
{
	//TODO: encrypt password!!
	return _password;
}

std::string LoginRequest::getUsername()
{
	return userName;
}

std::string LoginRequest::getPassword()
{
	return password;
}

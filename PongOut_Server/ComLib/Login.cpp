#include "stdafx.h"
#include "Login.h"


Login::Login(void) : msgBase(msgBase::MsgType::LOGIN)
{
}


Login::~Login(void)
{
}

std::vector<char> Login::getData()
{
	std::vector<char> res;
	std::back_insert_iterator<std::vector<char>> it(res);
	pack(msgHeader, it);
	pack(userName, it);
	pack(password, it);
	return res;
}

msgBase::ptr Login::interpretPacket( const std::deque<char>& _buffer )
{
	Login::ptr lp = Login::ptr(new Login());
	std::deque<char>::const_iterator it = _buffer.cbegin();
	it = unpack(lp->msgHeader, it);
	it = unpack(lp->userName, it);
	it = unpack(lp->password, it);
	return lp;
}

void Login::setLogin( const std::string& _userName, const std::string& _password )
{
	userName = _userName;
	password = encryptPassword(_password);
	msgHeader.length = userName.length() + password.length() + sizeof(std::uint16_t) * 2;
}

std::string Login::encryptPassword( std::string _password )
{
	//TODO: encrypt password!!
	return _password;
}

std::string Login::getUsername()
{
	return userName;
}

std::string Login::getPassword()
{
	return password;
}

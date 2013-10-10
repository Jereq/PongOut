#include "stdafx.h"
#include "RequestCreateUser.h"


RequestCreateUser::RequestCreateUser(void) :msgBase(msgBase::MsgType::REQUESTCREATEUSER)
{
}


RequestCreateUser::~RequestCreateUser(void)
{
}

void RequestCreateUser::setCredentials( const std::string& _userName, const std::string& _userPawwsord )
{
	uName = _userName;
	uPass = _userPawwsord;
	msgHeader.length = uName.length() + uPass.length() + (sizeof(std::uint16_t) * 2);
}

std::vector<char> RequestCreateUser::getData()
{
	std::vector<char> res;
	std::back_insert_iterator<std::vector<char>> iter(res);
	pack(msgHeader, iter);
	pack(uName, iter);
	pack(uPass, iter);

	return res;
}

msgBase::ptr RequestCreateUser::interpretPacket( const std::deque<char>& _buffer )
{
	RequestCreateUser::ptr cu = RequestCreateUser::ptr(new RequestCreateUser());
	std::deque<char>::const_iterator it = _buffer.begin();

	it = unpack(cu->msgHeader, it);
	it = unpack(cu->uName, it);
	it = unpack(cu->uPass, it);

	return cu;
}

std::string RequestCreateUser::getUserName()
{
	return uName;
}

std::string RequestCreateUser::getUserPassword()
{
	return uPass;
}

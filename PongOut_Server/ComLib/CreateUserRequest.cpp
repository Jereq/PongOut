#include "stdafx.h"
#include "CreateUserRequest.h"


CreateUserRequest::CreateUserRequest(void) :msgBase(msgBase::MsgType::CREATEUSERREQUEST)
{
}


CreateUserRequest::~CreateUserRequest(void)
{
}

void CreateUserRequest::setCredentials( const std::string& _userName, const std::string& _userPawwsord )
{
	uName = _userName;
	uPass = _userPawwsord;
	msgHeader.length = uName.length() + uPass.length() + (sizeof(std::uint16_t) * 2);
}

std::vector<char> CreateUserRequest::getData()
{
	std::vector<char> res;
	std::back_insert_iterator<std::vector<char>> iter(res);
	pack(msgHeader, iter);
	pack(uName, iter);
	pack(uPass, iter);

	return res;
}

msgBase::ptr CreateUserRequest::interpretPacket( const std::deque<char>& _buffer )
{
	CreateUserRequest::ptr cu = CreateUserRequest::ptr(new CreateUserRequest());
	std::deque<char>::const_iterator it = _buffer.begin();

	it = unpack(cu->msgHeader, it);
	it = unpack(cu->uName, it);
	it = unpack(cu->uPass, it);

	return cu;
}

std::string CreateUserRequest::getUserName()
{
	return uName;
}

std::string CreateUserRequest::getUserPassword()
{
	return uPass;
}

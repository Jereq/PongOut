#include "stdafx.h"
#include "CreateUserResponse.h"


CreateUserResponse::CreateUserResponse(void) : msgBase(msgBase::MsgType::CREATEUSERRESPONSE)
{
}


CreateUserResponse::~CreateUserResponse(void)
{
}

void CreateUserResponse::setCreateFailure( bool _failure )
{
	createFailure = _failure;
	msgHeader.length = sizeof(char);
}

bool CreateUserResponse::getCreateFailure()
{
	return createFailure;
}

std::vector<char> CreateUserResponse::getData()
{
	std::vector<char> res;
	std::back_insert_iterator<std::vector<char>> iter(res);
	pack(msgHeader, iter);
	pack(createFailure, iter);

	return res;
}

msgBase::ptr CreateUserResponse::interpretPacket( const std::deque<char>& _buffer )
{
	CreateUserResponse::ptr rcu = CreateUserResponse::ptr(new CreateUserResponse());
	std::deque<char>::const_iterator it = _buffer.cbegin();
	it = unpack(rcu->msgHeader, it);
	it = unpack(rcu->createFailure, it);

	return rcu;
}
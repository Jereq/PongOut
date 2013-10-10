#include "stdafx.h"
#include "ResponseCreateUser.h"


ResponseCreateUser::ResponseCreateUser(void) : msgBase(msgBase::MsgType::RESPONSECREATEUSER)
{
}


ResponseCreateUser::~ResponseCreateUser(void)
{
}

void ResponseCreateUser::setCreateFailure( bool _failure )
{
	createFailure = _failure;
	msgHeader.length = sizeof(char);
}

bool ResponseCreateUser::getCreateFailure()
{
	return createFailure;
}

std::vector<char> ResponseCreateUser::getData()
{
	std::vector<char> res;
	std::back_insert_iterator<std::vector<char>> iter(res);
	pack(msgHeader, iter);
	pack(createFailure, iter);

	return res;
}

msgBase::ptr ResponseCreateUser::interpretPacket( const std::deque<char>& _buffer )
{
	ResponseCreateUser::ptr rcu = ResponseCreateUser::ptr(new ResponseCreateUser());
	std::deque<char>::const_iterator it = _buffer.cbegin();
	it = unpack(rcu->msgHeader, it);
	it = unpack(rcu->createFailure, it);

	return rcu;
}
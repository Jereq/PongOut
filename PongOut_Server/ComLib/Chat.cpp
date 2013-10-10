#include "stdafx.h"
#include "Chat.h"

Chat::Chat(void) : msgBase(msgBase::MsgType::CHAT)
{
}


Chat::~Chat(void)
{
}

void Chat::setMsg( std::string _msg, unsigned int _id, std::string _userName)
{
	id = _id;
	msg = _msg;
	user = _userName;
	msgHeader.length = msg.length() + user.length() + (sizeof(std::uint16_t) * 2) + sizeof(unsigned int);
}

std::vector<char> Chat::getData()
{
	std::vector<char> res;
	std::back_insert_iterator<std::vector<char>> iter(res);
	pack(msgHeader, iter);
	pack(user, iter);
	pack(id, iter);
	pack(msg, iter);

	return res;
}

msgBase::ptr Chat::interpretPacket( const std::deque<char>& _buffer )
{
	Chat::ptr cp = Chat::ptr(new Chat());
	std::deque<char>::const_iterator it = _buffer.cbegin();
	it = unpack(cp->msgHeader, it);
	it = unpack(cp->user, it);
	it = unpack(cp->id, it);
	it = unpack(cp->msg, it);
	return cp;
}

std::string Chat::getMsg()
{
	return msg;
}

unsigned int Chat::getUserID()
{
	return id;
}

void Chat::setUserID(unsigned int _id )
{
	id = _id;
}

void Chat::setUserName( const std::string& _userName )
{
	user = _userName;
}

std::string Chat::getUserName()
{
	return user;
}

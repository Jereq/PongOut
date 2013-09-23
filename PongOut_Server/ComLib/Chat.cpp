#include "stdafx.h"
#include "Chat.h"

Chat::Chat(void) : msgBase(msgBase::MsgType::CHAT)
{
}


Chat::~Chat(void)
{
}

void Chat::setMsg( std::string _msg, boost::uuids::uuid& _uuid)
{
	uuid = _uuid;
	msg = _msg;
	msgHeader.length = msg.length() + sizeof(std::uint16_t) + boost::uuids::uuid::static_size();
}

std::vector<char> Chat::getData()
{
	std::vector<char> res;
	std::back_insert_iterator<std::vector<char>> iter(res);
	pack(msgHeader, iter);
	pack(uuid, iter);
	pack(msg, iter);

	return res;
}

msgBase::ptr Chat::interpretPacket( const std::deque<char>& _buffer )
{
	Chat::ptr cp = Chat::ptr(new Chat());
	std::deque<char>::const_iterator it = _buffer.cbegin();
	it = unpack(cp->msgHeader, it);
	it = unpack(cp->uuid, it);
	it = unpack(cp->msg, it);
	return cp;
}

std::string Chat::getMsg()
{
	return msg;
}

boost::uuids::uuid Chat::getUUID()
{
	return uuid;
}

#include "stdafx.h"
#include "PacketHandler.h"
#include "Chat.h"
#include "Login.h"
#include "ResponseFriendlist.h"
#include "RequestFriendlist.h"

PacketHandler& PacketHandler::getInstance()
{
	static PacketHandler instance;
	return instance;
}

PacketHandler::PacketHandler(void)
{
}

PacketHandler::~PacketHandler(void)
{
}

void PacketHandler::registerPacket( msgBase::ptr _ptr )
{
	msgMap.insert(std::pair<msgBase::MsgType, msgBase::ptr>(_ptr->getHeader().type, _ptr));
}

msgBase::ptr PacketHandler::interpretMessage( msgBase::MsgType _type, const std::deque<char>& _buff )
{
	return msgMap.at(_type)->interpretPacket(_buff);
}

void PacketHandler::initRegister()
{
	registerPacket(msgBase::ptr(new Chat()));
	registerPacket(msgBase::ptr(new ResponseFriendlist()));
	registerPacket(msgBase::ptr(new Login()));
	registerPacket(msgBase::ptr(new RequestFriendlist()));
}

msgBase::header PacketHandler::getMeassageHeader( const std::vector<char>& _buff )
{
	msgBase::header	res;
	std::vector<char>::const_iterator it = _buff.begin();
	it = msgBase::unpack(res, it);
	return res;
}

msgBase::header PacketHandler::getMeassageHeader( const boost::array<char, 256>& _buff )
{
	msgBase::header	res;
	boost::array<char, 256>::const_iterator it = _buff.begin();
	it = msgBase::unpack(res, it);
	return res;
}

msgBase::header PacketHandler::getMeassageHeader( const std::deque<char>& _buff )
{
	msgBase::header	res;
	std::deque<char>::const_iterator it = _buff.begin();
	it = msgBase::unpack(res, it);
	return res;
}

#include "stdafx.h"
#include "PacketHandler.h"
#include "Chat.h"

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

msgBase::ptr PacketHandler::interpretMessage( msgBase::MsgType _type, const std::vector<char>& _buff )
{	
	return msgMap.at(_type)->createPacket(_buff);
}

void PacketHandler::initRegister()
{
	registerPacket(msgBase::ptr(new Chat()));
}

msgBase::header PacketHandler::getMeassageHeader( const std::vector<char>& _buff )
{
	msgBase::header	res;
	std::vector<char>::const_iterator it = _buff.begin();
	it = msgBase::unpack(res, it);
	return res;
}

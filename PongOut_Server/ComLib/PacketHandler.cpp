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

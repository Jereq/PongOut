#include "stdafx.h"
#include "PacketHandler.h"
#include "Chat.h"
#include "RequestLogin.h"
#include "ResponseLogin.h"
#include "ResponseFriendlist.h"
#include "RequestFriendlist.h"
#include "RequestCreateUser.h"
#include "RequestLogout.h"
#include "ResponseCreateUser.h"
#include "ResponseConnect.h"

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
	registerPacket(msgBase::ptr(new RequestLogin()));
	registerPacket(msgBase::ptr(new ResponseLogin()));
	registerPacket(msgBase::ptr(new RequestFriendlist()));
	registerPacket(msgBase::ptr(new RequestCreateUser()));
	registerPacket(msgBase::ptr(new RequestLogout()));
	registerPacket(msgBase::ptr(new ResponseCreateUser()));
	registerPacket(msgBase::ptr(new ResponseConnect()));
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

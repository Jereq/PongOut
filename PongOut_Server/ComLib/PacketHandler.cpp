#include "stdafx.h"
#include "PacketHandler.h"
#include "Chat.h"
#include "LoginRequest.h"
#include "LoginResponse.h"
#include "FriendlistResponse.h"
#include "FriendlistRequest.h"
#include "CreateUserRequest.h"
#include "LogoutRequest.h"
#include "CreateUserResponse.h"
#include "ConnectResponse.h"
#include "GameMessage.h"

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
	registerPacket(msgBase::ptr(new FriendlistResponse()));
	registerPacket(msgBase::ptr(new LoginRequest()));
	registerPacket(msgBase::ptr(new LoginResponse()));
	registerPacket(msgBase::ptr(new FriendlistRequest()));
	registerPacket(msgBase::ptr(new CreateUserRequest()));
	registerPacket(msgBase::ptr(new LogoutRequest()));
	registerPacket(msgBase::ptr(new CreateUserResponse()));
	registerPacket(msgBase::ptr(new ConnectResponse()));
	registerPacket(msgBase::ptr(new GameMessage()));
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

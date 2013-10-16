#include "stdafx.h"
#include "PacketHandler.h"
#include "Chat.h"
#include "LoginRequest.h"
#include "FriendlistResponse.h"
#include "FriendlistRequest.h"
#include "CreateUserRequest.h"
#include "LogoutRequest.h"
#include "AcknowledgeLast.h"
#include "GameMessage.h"
#include "CreateGame.h"

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
	registerPacket(msgBase::ptr(new FriendlistRequest()));
	registerPacket(msgBase::ptr(new CreateUserRequest()));
	registerPacket(msgBase::ptr(new LogoutRequest()));
	registerPacket(msgBase::ptr(new AcknowledgeLast()));
	
	GameMessage::ptr gameMsg(new GameMessage());

	gameMsg->registerChild(GameMessage::ptr(new CreateGame()));

	registerPacket(gameMsg);
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

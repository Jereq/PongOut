#include "stdafx.h"
#include "GameMessage.h"


GameMessage::GameMessage() : msgBase(msgBase::MsgType::GAMEMESSAGE)
{
	
}


GameMessage::~GameMessage(void)
{
}

void GameMessage::registerChild( GameMessage::ptr _childPtr )
{
	gameMsgMap.insert(std::pair<GameMessage::GameMsgType, GameMessage::ptr>(_childPtr->getGameType(), _childPtr));
}

std::vector<char> GameMessage::getData()
{
	return std::vector<char>();
}

msgBase::ptr GameMessage::interpretPacket( const std::deque<char>& _buffer )
{
	std::deque<char>::const_iterator it = _buffer.begin() + sizeof(header);

	GameMsgType childHead;

	it = unpack(childHead, it);		

	return gameMsgMap.at(childHead)->interpretPacket(_buffer);
}

GameMessage::GameMsgType GameMessage::getGameType()
{
	return gType;
}

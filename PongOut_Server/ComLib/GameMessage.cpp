#include "stdafx.h"
#include "GameMessage.h"


GameMessage::GameMessage(GameMsgType _type ) : msgBase(msgBase::MsgType::GAMEMESSAGE)
{
	type = _type;
}


GameMessage::~GameMessage(void)
{
}

std::vector<char> GameMessage::getData()
{
	std::vector<char> res;
	std::back_insert_iterator<std::vector<char>> iter(res);
	pack(msgHeader, iter);

	return res;
}

msgBase::ptr GameMessage::interpretPacket( const std::deque<char>& _buffer )
{
	GameMessage::ptr gmp = GameMessage::ptr(new GameMessage());
	std::deque<char>::const_iterator it = _buffer.begin();

	it = unpack(gmp->msgHeader, it);

	return gmp;
}
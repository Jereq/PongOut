#include "stdafx.h"
#include "EndGameRequest.h"


EndGameRequest::EndGameRequest(void) : GameMessage()
{
	gType = GameMessage::GameMsgType::END_GAME_REQUEST;
	msgHeader.length = sizeof(GameMsgType);
}


EndGameRequest::~EndGameRequest(void)
{
}

std::vector<char> EndGameRequest::getData()
{
	std::vector<char> res;
	std::back_insert_iterator<std::vector<char>> it(res);

	pack(msgHeader, it);
	pack(gType, it);

	return res;
}

msgBase::ptr EndGameRequest::interpretPacket( const std::deque<char>& _buffer )
{
	EndGameRequest::ptr egr = EndGameRequest::ptr(new EndGameRequest());
	std::deque<char>::const_iterator it = _buffer.cbegin();

	it = unpack(egr->msgHeader, it);
	it = unpack(egr->gType, it);

	return egr;
}

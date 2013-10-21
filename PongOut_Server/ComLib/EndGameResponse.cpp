#include "stdafx.h"
#include "EndGameResponse.h"


EndGameResponse::EndGameResponse(void) : GameMessage()
{
	gType = GameMessage::GameMsgType::END_GAME_RESPONSE;
}


EndGameResponse::~EndGameResponse(void)
{
}

void EndGameResponse::setResult( CommonTypes::MatchResult _res )
{
	res = _res;
	msgHeader.length = sizeof(CommonTypes::MatchResult) + sizeof(GameMsgType);
}

CommonTypes::MatchResult EndGameResponse::getResult()
{
	return res;
}

std::vector<char> EndGameResponse::getData()
{
	std::vector<char> result;
	std::back_insert_iterator<std::vector<char>> it(result);

	pack(msgHeader, it);
	pack(gType, it);
	pack(res, it);

	return result;
}

msgBase::ptr EndGameResponse::interpretPacket( const std::deque<char>& _buffer )
{
	EndGameResponse::ptr egr = EndGameResponse::ptr(new EndGameResponse());
	std::deque<char>::const_iterator it = _buffer.cbegin();

	it = unpack(egr->msgHeader, it);
	it = unpack(egr->gType, it);
	it = unpack(egr->res, it);

	return egr;
}

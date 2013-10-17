#include "stdafx.h"
#include "CreateGameResponse.h"


CreateGameResponse::CreateGameResponse(void) : GameMessage()
{
	gType = GameMessage::GameMsgType::CREATEGAMERESPONSE;
}


CreateGameResponse::~CreateGameResponse(void)
{
}

void CreateGameResponse::setResponse( const std::vector<CommonTypes::Block>& _map, const CommonTypes::GameInitInfo& _info )
{
	map = _map;
	info = _info;
}

std::vector<char> CreateGameResponse::getData()
{
	std::vector<char> res;
	std::back_insert_iterator<std::vector<char>> it(res);

	pack(msgHeader, it);
	pack(gType, it);
	pack(info, it);
	pack(map, it);

	return res;
}

msgBase::ptr CreateGameResponse::interpretPacket( const std::deque<char>& _buffer )
{
	CreateGameResponse::ptr cgp = CreateGameResponse::ptr(new CreateGameResponse());
	std::deque<char>::const_iterator it = _buffer.cbegin();

	it = unpack(cgp->msgHeader, it);
	it = unpack(cgp->gType, it);
	it = unpack(cgp->info, it);
	it = unpack(cgp->map, it);

	return cgp;
}

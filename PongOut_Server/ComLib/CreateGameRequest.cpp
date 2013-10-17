#include "stdafx.h"
#include "CreateGameRequest.h"


CreateGameRequest::CreateGameRequest(void) : GameMessage()
{
	gType = GameMessage::GameMsgType::CREATEGAMEREQUEST;
}


CreateGameRequest::~CreateGameRequest(void)
{
}

CommonTypes::GameInitInfo CreateGameRequest::getInitInfo()
{
	return info;
}

void CreateGameRequest::setGameParam( CommonTypes::GameInitInfo _info )
{
	info = _info;
	msgHeader.length = sizeof(CommonTypes::GameInitInfo) + sizeof(GameMsgType);
}

std::vector<char> CreateGameRequest::getData()
{
	std::vector<char> res;
	std::back_insert_iterator<std::vector<char>> it(res);

	pack(msgHeader, it);
	pack(gType, it);
	pack(info, it);

	return res;
}

msgBase::ptr CreateGameRequest::interpretPacket( const std::deque<char>& _buffer )
{
	CreateGameRequest::ptr cgp = CreateGameRequest::ptr(new CreateGameRequest());
	std::deque<char>::const_iterator it = _buffer.cbegin();

	it = unpack(cgp->msgHeader, it);
	it = unpack(cgp->gType, it);
	it = unpack(cgp->info, it);

	return cgp;
}
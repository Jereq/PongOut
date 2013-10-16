#include "stdafx.h"
#include "CreateGame.h"


CreateGame::CreateGame(void) : GameMessage()
{
	gType = GameMessage::GameMsgType::INITGAMEREQUEST;
}


CreateGame::~CreateGame(void)
{
}

CreateGame::GameInitInfo CreateGame::getInitInfo()
{
	return info;
}

void CreateGame::setGameParam( GameInitInfo _info )
{
	info = _info;
	msgHeader.length = sizeof(GameInitInfo) + sizeof(GameMsgType);
}

std::vector<char> CreateGame::getData()
{
	std::vector<char> res;
	std::back_insert_iterator<std::vector<char>> it(res);

	pack(msgHeader, it);
	pack(gType, it);
	pack(info, it);

	return res;
}

msgBase::ptr CreateGame::interpretPacket( const std::deque<char>& _buffer )
{
	CreateGame::ptr cgp = CreateGame::ptr(new CreateGame());
	std::deque<char>::const_iterator it = _buffer.cbegin();

	it = unpack(cgp->msgHeader, it);
	it = unpack(cgp->gType, it);
	it = unpack(cgp->info, it);

	return cgp;
}
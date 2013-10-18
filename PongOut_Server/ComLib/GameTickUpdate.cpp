#include "stdafx.h"
#include "GameTickUpdate.h"


GameTickUpdate::GameTickUpdate(void) : GameMessage()
{
	gType = GameMessage::GameMsgType::GAME_TICK_UPDATE;
}


GameTickUpdate::~GameTickUpdate(void)
{
}

void GameTickUpdate::setTickUpdate( CommonTypes::PlayerMatchInfo _pmiME, CommonTypes::PlayerMatchInfo _pmiOP, std::vector<CommonTypes::Block> _blocks )
{
	pmiME = _pmiME;
	pmiOP = _pmiOP;
	blocks = _blocks;

	int mapSize = 0;

	for (CommonTypes::Block b : blocks)
	{
		mapSize += b.getSize();
	}

	mapSize += sizeof(uint16_t);

	msgHeader.length = sizeof(CommonTypes::PlayerMatchInfo) * 2 + sizeof(GameMsgType) + mapSize;
}

CommonTypes::PlayerMatchInfo GameTickUpdate::getMyInfo()
{
	return pmiME;
}

CommonTypes::PlayerMatchInfo GameTickUpdate::getOpInfo()
{
	return pmiOP;
}

std::vector<CommonTypes::Block> GameTickUpdate::getBlockList()
{
	return blocks;
}

std::vector<char> GameTickUpdate::getData()
{
	std::vector<char> res;
	std::back_insert_iterator<std::vector<char>> it(res);

	pack(msgHeader, it);
	pack(gType, it);
	pack(pmiME, it);
	pack(pmiOP, it);
	pack(blocks, it);

	return res;
}

msgBase::ptr GameTickUpdate::interpretPacket( const std::deque<char>& _buffer )
{
	GameTickUpdate::ptr gtup = GameTickUpdate::ptr(new GameTickUpdate());
	std::deque<char>::const_iterator it = _buffer.cbegin();

	it = unpack(gtup->msgHeader, it);
	it = unpack(gtup->gType, it);
	it = unpack(gtup->pmiME, it);
	it = unpack(gtup->pmiOP, it);
	it = unpack(gtup->blocks, it);

	return gtup;
}
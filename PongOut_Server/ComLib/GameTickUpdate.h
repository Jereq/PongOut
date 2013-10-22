#pragma once

#include "GameMessage.h"

class GameTickUpdate : public GameMessage
{
public:

	typedef boost::shared_ptr<GameTickUpdate> ptr;

	GameTickUpdate(void);
	virtual ~GameTickUpdate(void);

	virtual std::vector<char> getData() override;
	virtual msgBase::ptr interpretPacket(const std::deque<char>& _buffer) override;

	void setTickUpdate(CommonTypes::PlayerMatchInfo _pmiME, CommonTypes::PlayerMatchInfo _pmiOP, std::vector<CommonTypes::Block> _blocks, std::uint16_t  _sudenDeath);
	CommonTypes::PlayerMatchInfo getMyInfo();
	CommonTypes::PlayerMatchInfo getOpInfo();
	std::vector<CommonTypes::Block> getBlockList();

	std::uint16_t getSuddenDeathTime();

private:

	CommonTypes::PlayerMatchInfo pmiME, pmiOP;
	std::vector<CommonTypes::Block> blocks;
	std::uint16_t suddenDeathTime;
};


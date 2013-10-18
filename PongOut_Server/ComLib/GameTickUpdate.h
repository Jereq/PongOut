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

	void setTickUpdate(CommonTypes::PlayerMatchInfo _pmiME, CommonTypes::PlayerMatchInfo _pmiOP, std::vector<CommonTypes::Block> _blocks);
	CommonTypes::PlayerMatchInfo getMyInfo();
	CommonTypes::PlayerMatchInfo getOpInfo();
	std::vector<CommonTypes::Block> getBlockList();

private:

	CommonTypes::PlayerMatchInfo pmiME, pmiOP;
	std::vector<CommonTypes::Block> blocks;
};


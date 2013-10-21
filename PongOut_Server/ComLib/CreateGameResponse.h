#pragma once

#include "gamemessage.h"

class CreateGameResponse :	public GameMessage
{
public:

	typedef boost::shared_ptr<CreateGameResponse> ptr;

	CreateGameResponse(void);
	virtual ~CreateGameResponse(void);

	virtual std::vector<char> getData() override;
	virtual msgBase::ptr interpretPacket(const std::deque<char>& _buffer) override;

	void setResponse(const std::vector<CommonTypes::Block>& _map, const CommonTypes::GameInitInfo& _info, const CommonTypes::PlayerMatchInfo& _pmiME, const CommonTypes::PlayerMatchInfo& _pmiOP);

	std::vector<CommonTypes::Block> getMap();
	CommonTypes::PlayerMatchInfo getPMIme();
	CommonTypes::PlayerMatchInfo getPMIop();

private:

	std::vector<CommonTypes::Block> map;
	CommonTypes::GameInitInfo info;
	CommonTypes::PlayerMatchInfo pmiME, pmiOP;
};


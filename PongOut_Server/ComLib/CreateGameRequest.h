#pragma once

#include "gamemessage.h"

class CreateGameRequest : 	public GameMessage
{
public:

	typedef boost::shared_ptr<CreateGameRequest> ptr;

	CreateGameRequest(void);
	virtual ~CreateGameRequest(void);

	virtual std::vector<char> getData() override;
	virtual msgBase::ptr interpretPacket(const std::deque<char>& _buffer) override;

	void setGameParam(GameInitInfo _info);

	GameInitInfo getInitInfo();


private:

	GameInitInfo info;

};


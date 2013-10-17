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

	void setGameParam(CommonTypes::GameInitInfo _info);

	CommonTypes::GameInitInfo getInitInfo();


private:

	CommonTypes::GameInitInfo info;

};


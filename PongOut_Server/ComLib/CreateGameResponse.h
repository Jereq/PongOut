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

	void setResponse(const std::vector<GameMessage::Block>& _map, const GameMessage::GameInitInfo& _info);

private:

	std::vector<GameMessage::Block> map;
	GameMessage::GameInitInfo info;

};


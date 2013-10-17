#pragma once
#include "GameMessage.h"
class CreateGame : 	public GameMessage
{
public:

	typedef boost::shared_ptr<CreateGame> ptr;

	struct GameInitInfo
	{
		int mapID, ballSpeed, paddleSpeed, suddenDeathTime;
		char fogOfWar, powerUps;
	};

	CreateGame(void);
	virtual ~CreateGame(void);

	virtual std::vector<char> getData() override;
	virtual msgBase::ptr interpretPacket(const std::deque<char>& _buffer) override;

	void setGameParam(GameInitInfo _info);

	GameInitInfo getInitInfo();


private:

	GameInitInfo info;

};


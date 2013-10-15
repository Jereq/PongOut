#pragma once

#include "msgbase.h"

class GameMessage :	public msgBase
{
public:

	typedef boost::shared_ptr<GameMessage> ptr;

	enum class GameMsgType : std::uint16_t
	{
		//CLIENT
		CREATEGAMEREQUEST,
		PADDLEUPDATEREQUEST,		

		//SERVER
		BALLUPDATEREQUEST,
		BLOCKUPDATEREQUEST,
		INITGAMEREQUEST,

		//COMMON
		ACKNOWLEDGELAST,
	};

	GameMessage(GameMsgType _type);
	virtual ~GameMessage(void);

	virtual std::vector<char> getData() override;
	virtual msgBase::ptr interpretPacket(const std::deque<char>& _buffer) override;

protected:

	GameMsgType type;
};


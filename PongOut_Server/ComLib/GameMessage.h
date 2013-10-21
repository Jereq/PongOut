#pragma once

#include <map>

#include "msgBase.h"

class GameMessage :	public msgBase
{
public:

	typedef boost::shared_ptr<GameMessage> ptr;

	enum class GameMsgType : std::uint16_t
	{
		//CLIENT
		CREATEGAMEREQUEST,
		PADDLEUPDATEREQUEST,
		END_GAME_REQUEST,

		//SERVER
		GAME_TICK_UPDATE,
		CREATEGAMERESPONSE,
		END_GAME_RESPONSE,

		//COMMON
		ACKNOWLEDGELAST,
	};

	GameMessage(void);
	virtual ~GameMessage(void);

	virtual std::vector<char> getData() override;
	virtual msgBase::ptr interpretPacket(const std::deque<char>& _buffer) override;

	void registerChild( GameMessage::ptr _childPtr );
	GameMsgType getGameType();

protected:

	GameMsgType gType;

private:

	std::map<GameMsgType, ptr> gameMsgMap;
};


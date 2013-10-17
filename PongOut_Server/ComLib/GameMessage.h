#pragma once

#include <map>

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

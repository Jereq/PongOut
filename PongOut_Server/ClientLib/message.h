#pragma once

#include "msgBase.h"

struct message
{
	msgBase::MsgType type;
	GameMessage::GameMsgType gType;
	msgBase::ptr msg;
	GameMessage::ptr gMsg;
	std::string strMsg;

	message(msgBase::MsgType _type, msgBase::ptr _msg)
	{
		type = _type;
		msg = _msg;
		strMsg = "";
		gMsg = GameMessage::ptr();
	}

	message(msgBase::MsgType _type, std::string _msg)
	{
		type = _type;
		msg = msgBase::ptr();
		strMsg = _msg;
		gMsg = GameMessage::ptr();
	}

	message(GameMessage::GameMsgType _type, GameMessage::ptr _msg)
	{
		gType = _type;
		gMsg = _msg;
		type = msgBase::MsgType::GAMEMESSAGE;
		msg = msgBase::ptr();
		strMsg = "";
	}
};
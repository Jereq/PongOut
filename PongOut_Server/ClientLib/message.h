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
	}

	message(msgBase::MsgType _type, std::string _msg)
	{
		type = _type;
		msg = msgBase::ptr();
		strMsg = _msg;
	}

	message(GameMessage::GameMsgType _type, GameMessage::ptr _msg)
	{
		gType = _type;
		gMsg = _msg;
		msg = msgBase::ptr();
		strMsg = "";
	}
};
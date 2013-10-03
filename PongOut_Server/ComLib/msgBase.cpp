#include "stdafx.h"
#include "msgBase.h"


msgBase::msgBase(MsgType _type)
{
	msgHeader.type = _type;

	if (typeMap.size() == 0)
	{
		typeMap.insert(std::pair<MsgType, std::string>(MsgType::CHAT, "CHAT"));
		typeMap.insert(std::pair<MsgType, std::string>(MsgType::LOGIN, "LOGIN"));
		typeMap.insert(std::pair<MsgType, std::string>(MsgType::LOGOUT, "LOGOUT"));
		typeMap.insert(std::pair<MsgType, std::string>(MsgType::CREATEUSER, "CREATEUSER"));
		typeMap.insert(std::pair<MsgType, std::string>(MsgType::REMINDUSER, "REMINDUSER"));
		typeMap.insert(std::pair<MsgType, std::string>(MsgType::REQUESTFRIENDLIST, "REQUESTFRIENDLIST"));
		typeMap.insert(std::pair<MsgType, std::string>(MsgType::RESPONSEFRIENDLIST, "RESPONSEFRIENDLIST"));
	}
}


msgBase::~msgBase(void)
{
}

msgBase::header msgBase::getHeader()
{
	return msgHeader;
}

std::string msgBase::getType()
{
	return typeMap.at(msgHeader.type);
}

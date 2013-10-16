#include "stdafx.h"
#include "msgBase.h"


msgBase::msgBase(MsgType _type)
{
	msgHeader.type = _type;

	if (typeMap.size() == 0)
	{
		typeMap.insert(std::pair<MsgType, std::string>(MsgType::CHAT, "CHAT"));
		typeMap.insert(std::pair<MsgType, std::string>(MsgType::LOGINREQUEST, "LOGIN REQUEST"));
		typeMap.insert(std::pair<MsgType, std::string>(MsgType::LOGOUTREQUEST, "LOGOUT REQUEST"));
		typeMap.insert(std::pair<MsgType, std::string>(MsgType::CREATEUSERREQUEST, "CREATEUSER REQUEST"));
		typeMap.insert(std::pair<MsgType, std::string>(MsgType::REMINDUSER, "REMINDUSER"));
		typeMap.insert(std::pair<MsgType, std::string>(MsgType::FRIENDLISTREQUEST, "FRIENDLIST REQUEST"));
		typeMap.insert(std::pair<MsgType, std::string>(MsgType::FRIENDLISTRESPONSE, "FRIENDLIST RESPONSE"));
		typeMap.insert(std::pair<MsgType, std::string>(MsgType::GAMEMESSAGE, "GAME MESSAGE"));
		typeMap.insert(std::pair<MsgType, std::string>(MsgType::ACKNOWLEDGELAST, "ACKNOWLEDGE LAST"));
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

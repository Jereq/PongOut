#include "stdafx.h"
#include "msgBase.h"


msgBase::msgBase(MsgType _type)
{
	msgHeader.type = _type;

	if (typeMap.size() == 0)
	{
		typeMap.insert(std::pair<MsgType, std::string>(MsgType::CHAT, "CHAT"));
		typeMap.insert(std::pair<MsgType, std::string>(MsgType::REQUESTLOGIN, "LOGIN REQUEST"));
		typeMap.insert(std::pair<MsgType, std::string>(MsgType::RESPONSELOGIN, "LOGIN RESPONSE"));
		typeMap.insert(std::pair<MsgType, std::string>(MsgType::REQUESTLOGOUT, "LOGOUT REQUEST"));
		typeMap.insert(std::pair<MsgType, std::string>(MsgType::REQUESTCREATEUSER, "CREATEUSER REQUEST"));
		typeMap.insert(std::pair<MsgType, std::string>(MsgType::RESPONSECREATEUSER, "CREATEUSER RESPONSE"));
		typeMap.insert(std::pair<MsgType, std::string>(MsgType::REMINDUSER, "REMINDUSER"));
		typeMap.insert(std::pair<MsgType, std::string>(MsgType::REQUESTFRIENDLIST, "FRIENDLIST REQUEST"));
		typeMap.insert(std::pair<MsgType, std::string>(MsgType::RESPONSEFRIENDLIST, "FRIENDLIST RESPONSE"));
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

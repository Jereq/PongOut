#pragma once

#include "msgBase.h"
#include <map>
#include <vector>

class PacketHandler
{
public:
	PacketHandler(void);
	~PacketHandler(void);
	void initRegister();
	msgBase::ptr interpretMessage(msgBase::MsgType _type, const std::vector<char>& _buff);
	msgBase::header getMeassageHeader(const std::vector<char>& _buff);

private:
	std::map<msgBase::MsgType, msgBase::ptr> msgMap;
	void registerPacket(msgBase::ptr _ptr);	
};


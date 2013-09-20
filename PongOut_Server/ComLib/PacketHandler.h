#pragma once

#include "msgBase.h"
#include <map>
#include <vector>
#include <boost/array.hpp>
#include <queue>

class PacketHandler
{
public:
	PacketHandler(void);
	~PacketHandler(void);
	void initRegister();
	msgBase::ptr interpretMessage(msgBase::MsgType _type, const std::deque<char>& _buff);
	msgBase::header getMeassageHeader(const std::vector<char>& _buff);
	msgBase::header getMeassageHeader(const boost::array<char, 256>& _buff);
	msgBase::header getMeassageHeader(const std::deque<char>& _buff);

private:
	std::map<msgBase::MsgType, msgBase::ptr> msgMap;
	void registerPacket(msgBase::ptr _ptr);	
};


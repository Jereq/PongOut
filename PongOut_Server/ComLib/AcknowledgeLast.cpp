#include "stdafx.h"
#include "AcknowledgeLast.h"


AcknowledgeLast::AcknowledgeLast(void) : msgBase(msgBase::MsgType::ACKNOWLEDGELAST)
{
	msgHeader.length = 0;
}


AcknowledgeLast::~AcknowledgeLast(void)
{
}

void AcknowledgeLast::setAck( MsgType _type, bool _failure )
{
	ackType = _type;
	ackBool = _failure;
	msgHeader.length = sizeof(char) + sizeof(MsgType);
}

msgBase::MsgType AcknowledgeLast::getType()
{
	return ackType;
}

bool AcknowledgeLast::getBool()
{
	return ackBool != 0;
}

std::vector<char> AcknowledgeLast::getData()
{
	std::vector<char> res;
	std::back_insert_iterator<std::vector<char>> iter(res);
	pack(msgHeader, iter);
	pack(ackBool, iter);
	pack(ackType, iter);

	return res;
}

msgBase::ptr AcknowledgeLast::interpretPacket( const std::deque<char>& _buffer )
{
	AcknowledgeLast::ptr rc = AcknowledgeLast::ptr(new AcknowledgeLast());
	std::deque<char>::const_iterator it = _buffer.cbegin();
	it = unpack(rc->msgHeader, it);
	it = unpack(rc->ackBool, it);
	it = unpack(rc->ackType, it);

	return rc;
}
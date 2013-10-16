#pragma once
#include "msgbase.h"
class AcknowledgeLast :	public msgBase
{
public:

	typedef boost::shared_ptr<AcknowledgeLast> ptr;

	AcknowledgeLast(void);
	virtual ~AcknowledgeLast(void);

	virtual std::vector<char> getData() override;
	virtual msgBase::ptr interpretPacket(const std::deque<char>& _buffer) override;

	void setAck(MsgType _type, bool _failure);
	MsgType getType();
	bool getBool();

private:

	char ackBool;
	MsgType ackType;
};


#pragma once

#include "msgbase.h"

class ResponseCreateUser :	public msgBase
{
public:

	typedef boost::shared_ptr<ResponseCreateUser> ptr;

	ResponseCreateUser(void);
	virtual ~ResponseCreateUser(void);

	virtual std::vector<char> getData() override;
	virtual msgBase::ptr interpretPacket(const std::deque<char>& _buffer) override;

	void setCreateFailure(bool _failure);
	bool getCreateFailure();

private:

	char createFailure;
};


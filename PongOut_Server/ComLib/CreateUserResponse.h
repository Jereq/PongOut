#pragma once

#include "msgbase.h"

class CreateUserResponse :	public msgBase
{
public:

	typedef boost::shared_ptr<CreateUserResponse> ptr;

	CreateUserResponse(void);
	virtual ~CreateUserResponse(void);

	virtual std::vector<char> getData() override;
	virtual msgBase::ptr interpretPacket(const std::deque<char>& _buffer) override;

	void setCreateFailure(bool _failure);
	bool getCreateFailure();

private:

	char createFailure;
};


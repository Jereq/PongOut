#pragma once

#include "msgbase.h"

class LoginResponse : public msgBase
{
public:

	typedef boost::shared_ptr<LoginResponse> ptr;

	LoginResponse(void);
	virtual ~LoginResponse(void);

	virtual std::vector<char> getData() override;
	virtual msgBase::ptr interpretPacket(const std::deque<char>& _buffer) override;
	void setLoginFailure(bool _failure);
	bool getLoginFailure();

private:

	char loginFailure;
};


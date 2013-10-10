#pragma once

#include "msgbase.h"

class ResponseLogin : public msgBase
{
public:

	typedef boost::shared_ptr<ResponseLogin> ptr;

	ResponseLogin(void);
	virtual ~ResponseLogin(void);

	virtual std::vector<char> getData() override;
	virtual msgBase::ptr interpretPacket(const std::deque<char>& _buffer) override;
	void setLoginFailure(bool _failure);
	bool getLoginFailure();

private:

	char loginFailure;
};


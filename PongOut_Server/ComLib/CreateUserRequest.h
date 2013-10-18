#pragma once
#include "msgBase.h"
class CreateUserRequest :	public msgBase
{
public:

	typedef boost::shared_ptr<CreateUserRequest> ptr;

	CreateUserRequest(void);
	virtual ~CreateUserRequest(void);

	virtual std::vector<char> getData() override;
	virtual msgBase::ptr interpretPacket(const std::deque<char>& _buffer) override;

	void setCredentials(const std::string& _userName, const std::string& _userPawwsord);

	std::string getUserName();
	std::string getUserPassword();

	private:

	std::string uName, uPass;
};


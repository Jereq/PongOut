#pragma once
#include "msgbase.h"

class LoginRequest : public msgBase
{
public:

	typedef boost::shared_ptr<LoginRequest> ptr;

	LoginRequest(void);
	virtual ~LoginRequest(void);

	virtual std::vector<char> getData() override;
	virtual msgBase::ptr interpretPacket(const std::deque<char>& _buffer) override;

	void setLogin(const std::string& _userName, const std::string& _password);

	std::string getUsername();
	std::string getPassword();

private:

	std::string encryptPassword(std::string _password);

	std::string userName, password;
};


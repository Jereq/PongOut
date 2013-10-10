#pragma once
#include "msgbase.h"

class Chat : public msgBase
{
public:

	typedef boost::shared_ptr<Chat> ptr;

	Chat(void);
	~Chat(void);
	void setMsg(std::string _msg, unsigned int _id , std::string _userName);

	virtual std::vector<char> getData() override;
	virtual msgBase::ptr interpretPacket(const std::deque<char>& _buffer) override;
	std::string getMsg();
	unsigned int getUserID();
	std::string getUserName();
	void setUserID(unsigned int _id);
	void setUserName(const std::string& _userName);

private:

	unsigned int id;
	std::string msg, user;
};


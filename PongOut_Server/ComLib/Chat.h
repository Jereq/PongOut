#pragma once
#include "msgbase.h"

class Chat : public msgBase
{
public:

	typedef boost::shared_ptr<Chat> ptr;

	Chat(void);
	~Chat(void);
	void setMsg(std::string _msg, boost::uuids::uuid& _uuid, std::string _userName);

	virtual std::vector<char> getData() override;
	virtual msgBase::ptr interpretPacket(const std::deque<char>& _buffer) override;
	std::string getMsg();
	boost::uuids::uuid getUUID();
	std::string getUserName();
	void setUUID(const boost::uuids::uuid& _uuid);
	void setUserName(const std::string& _userName);

private:

	boost::uuids::uuid uuid;
	std::string msg, user;
};


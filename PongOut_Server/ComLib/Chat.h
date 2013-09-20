#pragma once
#include "msgbase.h"

class Chat : public msgBase
{
public:

	typedef boost::shared_ptr<Chat> ptr;

	Chat(void);
	~Chat(void);
	void setMsg(std::string _msg, boost::uuids::uuid& _uuid);

	virtual std::vector<char> getData() override;
	virtual msgBase::ptr interpretPacket(const std::deque<char>& _buffer) override;
	std::string getMsg();
	boost::uuids::uuid getUUID();

private:

	boost::uuids::uuid uuid;
	std::string msg;
};


#pragma once

#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <queue>

#include "msgbase.h"

using boost::asio::ip::tcp;

class User : public boost::enable_shared_from_this<User>
{
public:

	typedef boost::shared_ptr<User> ptr;

	User(tcp::socket* _socket, msgBase::userData _userData);
	~User(void);

	tcp::socket* getSocket();
	void addMsgToMsgQueue(msgBase::ptr _msgPtr);
	

private:
	tcp::socket* socket;
	void handleWrite(const boost::system::error_code& _err, size_t _byte);
	std::vector<char> buffer;
	void sendMsg();
	std::queue<msgBase::ptr> msgBuffer;
	std::mutex msgBufferLock;
	msgBase::userData userData;
};


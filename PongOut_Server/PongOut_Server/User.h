#pragma once

#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <queue>
#include <boost/array.hpp>
#include <boost/uuid/uuid.hpp>

#include "PacketHandler.h"
#include "msgbase.h"

class UserManager;

using boost::asio::ip::tcp;

class User : public boost::enable_shared_from_this<User>
{
public:

	typedef boost::shared_ptr<User> ptr;

	User(boost::shared_ptr<tcp::socket> _socket, boost::uuids::uuid _uuid);
	~User(void);

	boost::shared_ptr<tcp::socket> getSocket();
	void addMsgToMsgQueue(const msgBase::ptr& _msgPtr);
	void listen();
	msgBase::userData getUserData();
	void setUserNamePass(const std::string& _name, const std::string& _pass);

private:
	boost::shared_ptr<tcp::socket> socket;
	void handleWrite(const boost::system::error_code& _err, size_t _byte);
	void handleIncomingMeassage(const boost::system::error_code& _error, size_t _bytesTransferred);
	
	void sendMsg();

	std::queue<msgBase::ptr> msgWriteBufferQueue;
	boost::array<char, 256> msgListenBuffer;
	std::vector<char> msgWriteBuffer;
	std::deque<char> fullMsgBuffer;
	std::mutex msgBufferLock;
	msgBase::userData userData;
	msgBase::header head;
};


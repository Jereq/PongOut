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

#include "PacketHandler.h"
#include "msgbase.h"

class UserManager;

using boost::asio::ip::tcp;

class User : public boost::enable_shared_from_this<User>
{
public:

	enum class UserStatus
	{
		UNVERIFIED,
		USER,
		ADMIN
	};


	typedef boost::shared_ptr<User> ptr;

	User(boost::shared_ptr<tcp::socket> _socket);
	~User(void);

	boost::shared_ptr<tcp::socket> getSocket();
	void addMsgToMsgQueue(const msgBase::ptr& _msgPtr);
	void listen();
	unsigned int getUserID();
	void setUserStatus(UserStatus _status);
	UserStatus getUserStatus();
	void setUserID(unsigned int _id);
	int getRefereeID();
	void setReffereeID(int _refID);

	void disconnect();

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
	unsigned int id;
	int refID;
	msgBase::header head;
	UserStatus status;
};


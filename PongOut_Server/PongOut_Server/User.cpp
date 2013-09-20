#include "stdafx.h"
#include "User.h"


User::User(tcp::socket* _socket, msgBase::userData _userData)
	: socket(_socket), userData(_userData){}


User::~User(void)
{
	delete socket;
}

tcp::socket* User::getSocket()
{
	return socket;
}

void User::handleWrite( const boost::system::error_code& _err, size_t _byte )
{
	std::lock_guard<std::mutex> lock(msgBufferLock);
	msgBuffer.pop();

	if (!msgBuffer.empty())
	{
		sendMsg();
	}

	std::cerr << _err.message() << std::endl;
}

void User::sendMsg()
{
	buffer = msgBuffer.front()->getData();
	boost::asio::async_write(*socket, boost::asio::buffer(buffer), boost::bind(&User::handleWrite, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void User::addMsgToMsgQueue( msgBase::ptr _msgPtr )
{
	std::lock_guard<std::mutex> lock(msgBufferLock);
	msgBuffer.push(_msgPtr);
	if (msgBuffer.size() == 1)
	{
		sendMsg();
	}
}

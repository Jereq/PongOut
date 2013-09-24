#include "stdafx.h"
#include "User.h"


User::User(tcp::socket* _socket)
	: socket(_socket){}


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
	std::cerr << _err.message() << std::endl;
}

void User::sendMsg( msgBase::ptr _msgPtr )
{
	buffer = _msgPtr->getData();
	boost::asio::async_write(*socket, boost::asio::buffer(buffer), boost::bind(&User::handleWrite, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

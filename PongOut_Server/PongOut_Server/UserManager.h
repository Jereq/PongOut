#pragma once

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "SafeSet.h"
#include "User.h"

using boost::asio::ip::tcp;

class UserManager
{
public:
	UserManager(boost::asio::io_service* _io);
	~UserManager(void);

	boost::asio::io_service* ioService;
	SafeSet<User::ptr> users;
	tcp::acceptor acceptor;


	void listen();
	void handleAccept(tcp::socket* _soc, const boost::system::error_code& _errorCode );

};


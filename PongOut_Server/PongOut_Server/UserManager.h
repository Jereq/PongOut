#pragma once

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <PacketHandler.h>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <thread>
#include <SQLManager.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

#include "SafeSet.h"
#include "User.h"

using boost::asio::ip::tcp;

class UserManager : public boost::enable_shared_from_this<UserManager>
{
public:

	static boost::shared_ptr<UserManager> getInstance();
	~UserManager(void);
	SafeSet<User::ptr> users;

	void listenForNewClientConnections();
	void startIO();
	void handleIncomingClient(boost::shared_ptr<tcp::socket> _soc, const boost::system::error_code& _errorCode );
	void handleIncomingMeassage(const boost::system::error_code& _error, size_t _bytesTransferred);
	void messageActionSwitch(const msgBase::header& _header, const std::deque<char>& _meassage, boost::shared_ptr<User> _user);
	void destroy();
	bool checkIfIPAlowed(boost::asio::ip::address _addr);
	void addIPtoBlackList(boost::asio::ip::address _addr);

private:
	
	struct userIP
	{
		boost::posix_time::ptime firstConnect;
		int connectAtempts;
	};

	UserManager(void);	

	void startIOPrivate();
	bool userAllredyLogedin(long _uid);

	static boost::shared_ptr<UserManager> ptr;
	boost::shared_ptr<boost::asio::io_service> ioService;
	boost::shared_ptr<tcp::acceptor> acceptor;
	std::thread ioThread;
	SQLManager sqlManager;

	std::map<boost::asio::ip::address, userIP> ipBlackList;
};


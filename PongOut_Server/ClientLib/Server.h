#pragma once

#include <boost/asio.hpp>
#include <PacketHandler.h>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <thread>

#include "Chat.h"

using boost::asio::ip::tcp;

class Server : public boost::enable_shared_from_this<Server>
{
public:
	typedef boost::shared_ptr<Server> ptr;

	Server(const std::string _ipAdress, std::uint16_t _port);
	~Server(void);

	void connect();
	

private:

	void handleMessage(const boost::system::error_code& _error, size_t _bytesTransferred);
	void messageActionSwitch(const msgBase::header& _header, const std::vector<char>& _meassage);
	void startIO();
	void startListen();

	std::thread ioThread;
	const std::string addr;
	std::uint16_t port;
	boost::asio::io_service io;
	tcp::socket soc;
	tcp::resolver::iterator resIt;
	PacketHandler handler;
	boost::array<char, 256> msgBuffer;
};


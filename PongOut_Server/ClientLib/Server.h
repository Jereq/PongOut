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
#include "Login.h"
#include "ResponseFriendlist.h"
#include "RequestFriendlist.h"

using boost::asio::ip::tcp;

class Server : public boost::enable_shared_from_this<Server>
{
public:
	typedef boost::shared_ptr<Server> ptr;

	Server(const std::string _ipAdress, std::uint16_t _port);
	~Server(void);

	void connect(const std::string& _userName, const std::string& _password);
	void requestFriends();

private:

	void handleIncomingMessage(const boost::system::error_code& _error, size_t _bytesTransferred);
	void messageActionSwitch(const msgBase::header& _header, const std::deque<char>& _meassage);
	void startIO();
	void listen();
	void write(msgBase::ptr _msg);
	void handleWrite(const boost::system::error_code& _err, size_t _byte);

	std::thread ioThread;
	const std::string addr;
	std::uint16_t port;
	boost::asio::io_service io;
	tcp::socket soc;
	boost::array<char, 256> msgListenBuffer;
	std::vector<char> msgWriteBufffer;
	std::deque<char> fullMsgBuffer;
	msgBase::header head;
	std::vector<std::pair<std::string, boost::uuids::uuid>> friends;
};


#pragma once

#include <boost/asio.hpp>
#include <PacketHandler.h>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <thread>
#include <queue>

#include "Chat.h"
#include "RequestLogin.h"
#include "ResponseFriendlist.h"
#include "RequestFriendlist.h"
#include "RequestCreateUser.h"
#include "ResponseLogin.h"
#include "RequestLogout.h"
#include "ResponseCreateUser.h"
#include "ResponseConnect.h"
#include "SafeQueue.h"

using boost::asio::ip::tcp;

class Server : public boost::enable_shared_from_this<Server>
{
public:

	typedef boost::shared_ptr<Server> ptr;

	struct message
	{
		msgBase::MsgType type;
		msgBase::ptr msg;
		std::string strMsg;

		message(msgBase::MsgType _type, msgBase::ptr _msg)
		{
			type = _type;
			msg = _msg;
			strMsg = "";
		}

		message(msgBase::MsgType _type, std::string _msg)
		{
			type = _type;
			msg = msgBase::ptr();
			strMsg = _msg;
		}

	};

	Server(const std::string _ipAdress, std::uint16_t _port);
	~Server(void);

	void connect();
	void login(const std::string& _userName, const std::string& _password);
	void logout();
	void requestFriends();
	void sendChatMsg(std::string _name, std::string _msg);
	void createAccount(std::string _userName, std::string _userPassword);
	message getNextMessage();
	int getMsgQueueSize();

private:

	void handleIncomingMessage(const boost::system::error_code& _error, size_t _bytesTransferred);
	void messageActionSwitch(const msgBase::header& _header, const std::deque<char>& _meassage);
	void startIO();
	void listen();
	void write(msgBase::ptr _msg);
	void handleWrite(const boost::system::error_code& _err, size_t _byte);

	std::thread ioThread;
	const std::string addr;
	std::string userName;
	std::uint16_t port;
	boost::asio::io_service io;
	tcp::socket soc;
	boost::array<char, 256> msgListenBuffer;
	std::vector<char> msgWriteBufffer;
	std::deque<char> fullMsgBuffer;
	msgBase::header head;	
	SafeQueue<message> messages;
};


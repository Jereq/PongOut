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
#include "LoginRequest.h"
#include "FriendlistResponse.h"
#include "FriendlistRequest.h"
#include "CreateUserRequest.h"
#include "LogoutRequest.h"
#include "AcknowledgeLast.h"
#include "CreateGameRequest.h"
#include "GameMessage.h"
#include "CreateGameResponse.h"
#include "PaddleUpdateRequest.h"
#include "EndGameRequest.h"
#include "EndGameResponse.h"
#include "GameTickUpdate.h"
#include "LaunchBallRequest.h"

#include "SafeQueue.h"
#include "GameHandler.h"
#include "message.h"

#include <glm/glm.hpp>

using boost::asio::ip::tcp;

class Server : public boost::enable_shared_from_this<Server>
{
public:

	typedef boost::shared_ptr<Server> ptr;

	Server(const std::string _ipAdress, std::uint16_t _port);
	~Server(void);

	void connect();
	void login(const std::string& _userName, const std::string& _password);
	void logout();
	void requestFriends();
	void sendChatMsg(std::string _name, std::string _msg);
	void createAccount(std::string _userName, std::string _userPassword);
	void createGame(int _mapID, int _ballSpeed, int _paddleSpeed, int _suddenDeathTime, char _fogOfWar, char _powerUps);
	void sendPaddlePos(CommonTypes::Paddle _p);
	void sendLaunchBall();
	message getNextMessage();
	int getMsgQueueSize();

	void giveUpGame();

private:

	void handleIncomingMessage(const boost::system::error_code& _error, size_t _bytesTransferred);
	void messageActionSwitch(const msgBase::header& _header, const std::deque<char>& _meassage);
	void startIO();
	void listen();
	void write(msgBase::ptr _msg);
	void doWrite();
	void handleWrite(const boost::system::error_code& _err, size_t _byte);
	void connectResponse(const boost::system::error_code& _err);

	std::thread ioThread;
	const std::string addr;
	std::string userName;
	std::uint16_t port;
	boost::asio::io_service io;
	boost::shared_ptr<tcp::socket> soc;
	boost::array<char, 256> msgListenBuffer;
	std::mutex writeBufferMutex;
	std::queue<std::vector<char>> msgWriteBufffer;
	std::vector<char> currentWriteBuffer;
	std::deque<char> fullMsgBuffer;
	msgBase::header head;	
	SafeQueue<message> messages;
	bool isConnected;
	bool isWriting;
};


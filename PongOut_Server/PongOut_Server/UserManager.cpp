#include "stdafx.h"
#include "UserManager.h"
#include <Chat.h>
#include <Login.h>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

boost::shared_ptr<UserManager> UserManager::ptr;

boost::shared_ptr<UserManager> UserManager::getInstance()
{
	if (!ptr)
	{
		ptr = boost::shared_ptr<UserManager>(new UserManager());
	}
	return ptr;
}

UserManager::UserManager(void) 
			: ioService(new boost::asio::io_service()),
			  acceptor(new tcp::acceptor(*ioService, tcp::endpoint(tcp::v4(), 6500))) 
{
	PacketHandler::getInstance().initRegister();
}


UserManager::~UserManager(void)
{
}

void UserManager::listenForNewClientConnections()
{
	boost::shared_ptr<tcp::socket> socket(new tcp::socket(*ioService));

	acceptor->async_accept(*socket, boost::bind(&UserManager::handleIncomingClient, this, socket, boost::asio::placeholders::error));
}

void UserManager::handleIncomingClient( boost::shared_ptr<tcp::socket> _soc, const boost::system::error_code& _errorCode )
{
	boost::uuids::uuid uid = boost::uuids::random_generator()();
	User::ptr u = User::ptr(new User(_soc, uid));
	users.insert(u);
	u->listen();
	std::cout << "New user connected" << std::endl;
	listenForNewClientConnections();
}

void UserManager::messageActionSwitch( const msgBase::header& _header, const std::deque<char>& _meassage )
{
	msgBase::ptr p = PacketHandler::getInstance().interpretMessage(_header.type, _meassage);

	switch (_header.type)
	{
	case msgBase::MsgType::CHAT:
		{
			Chat::ptr cp = boost::static_pointer_cast<Chat>(p);
			break;
			//TODO: Call ClientEventLib chat event here when done!!
		}
	case msgBase::MsgType::LOGIN:
		{
			Login::ptr lp = boost::static_pointer_cast<Login>(p);
			std::cout << "Username: " << lp->getUsername() << "\tPassword: " << lp->getPassword() << std::endl;
			break;
		}
	default:
		std::cerr << "Received unknown packet: " << (int)p->getHeader().type << std::endl;
		break;
	}
}

void UserManager::startIO()
{
	ioThread = std::thread(boost::bind(&UserManager::startIOPrivate, shared_from_this()));
}

void UserManager::startIOPrivate()
{
	ioService->run();
	std::cout << "IO services stopped" << std::endl;
}
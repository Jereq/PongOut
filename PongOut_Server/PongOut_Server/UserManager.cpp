#include "stdafx.h"

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <Chat.h>
#include <Login.h>
#include <ResponseFriendlist.h>
#include <RequestFriendlist.h>
#include <stdexcept>
#include "UserManager.h"

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
	Log::addLog(Log::LogType::LOG_INFO, "New user connected");
	listenForNewClientConnections();
}

void UserManager::messageActionSwitch( const msgBase::header& _header, const std::deque<char>& _meassage, boost::shared_ptr<User> _user )
{
	msgBase::ptr p;

	try
	{
		p = PacketHandler::getInstance().interpretMessage(_header.type, _meassage);
	}
	catch(const std::out_of_range&)
	{
		Log::addLog(Log::LogType::LOG_ERROR, "Received unregistered packet: " + (int)_header.type);
		return;
	}

	switch (_header.type)
	{

	case msgBase::MsgType::CHAT:
		{
			Chat::ptr cp = boost::static_pointer_cast<Chat>(p);

			std::unique_lock<std::mutex> lock(users.getLock());
			User::ptr toUser;
			for (User::ptr user : users.baseSet)
			{
				if (user->getUserData().uuid == cp->getUUID())
				{
					toUser = user;
					break;
				}
			}

			cp->setUUID(_user->getUserData().uuid);
			toUser->addMsgToMsgQueue(cp);

			break;
			//TODO: Call ClientEventLib chat event here when done!!
		}

	case msgBase::MsgType::LOGIN:
		{
			Login::ptr lp = boost::static_pointer_cast<Login>(p);
			_user->setUserNamePass(lp->getUsername(), lp->getPassword());

			Log::addLog(Log::LogType::LOG_INFO, "Username: " + lp->getUsername() + " Logged in");

			break;
		}

	case msgBase::MsgType::REQUESTFRIENDLIST:
		{
			ResponseFriendlist::ptr rfl = ResponseFriendlist::ptr(new ResponseFriendlist());

			for (User::ptr user : users.baseSet)
			{
				std::pair<std::string, boost::uuids::uuid> tmpPair(user->getUserData().userName, user->getUserData().uuid);
				rfl->addToFriendList(tmpPair);
			}

			_user->addMsgToMsgQueue(rfl);

			break;
		}

	default:
		Log::addLog(Log::LogType::LOG_ERROR, "Received unknown packet: " + (int)p->getHeader().type);
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
	Log::addLog(Log::LogType::LOG_INFO, "IO services stopped");
}

void UserManager::destroy()
{
	ioService->stop();

	//std::unique_lock<std::mutex> lock(users.getLock());
	//for (auto u : users.baseSet)
	//{
	//	u->disconnect();
	//}
	//	
	//users.baseSet.clear();
	ioThread.join();
}

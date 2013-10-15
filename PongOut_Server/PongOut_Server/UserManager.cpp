#include "stdafx.h"
#include "UserManager.h"
#include "GameMaster.h"

#include <Chat.h>
#include <LoginRequest.h>
#include <LoginResponse.h>
#include <FriendlistResponse.h>
#include <FriendlistRequest.h>
#include <CreateUserRequest.h>
#include <CreateUserResponse.h>
#include <ConnectResponse.h>
#include <GameMessage.h>
#include <stdexcept>

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
	User::ptr u = User::ptr(new User(_soc));
	ConnectResponse::ptr rc = ConnectResponse::ptr(new ConnectResponse());
	u->addMsgToMsgQueue(rc);
	users.insert(u);
	u->listen();
	Log::addLog(Log::LogType::LOG_INFO, 4, "New connection established");
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
		Log::addLog(Log::LogType::LOG_ERROR, 0, "Received unregistered packet: " + (int)_header.type);
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
				if (user->getUserID() == cp->getUserID())
				{
					toUser = user;
					break;
				}
			}

			cp->setUserID(_user->getUserID());
			toUser->addMsgToMsgQueue(cp);

			break;
			//TODO: Call ClientEventLib chat event here when done!!
		}

	case msgBase::MsgType::LOGINREQUEST:
		{
			LoginRequest::ptr lp = boost::static_pointer_cast<LoginRequest>(p);
			LoginResponse::ptr rlp = LoginResponse::ptr(new LoginResponse());

			long res = sqlManager.verifyUser(lp->getUsername(), lp->getPassword());

			if (res != -1)
			{
				_user->setUserID(res);
				_user->setUserStatus(User::UserStatus::USER);
				Log::addLog(Log::LogType::LOG_INFO, 4, "Username: " + lp->getUsername() + " Logged in");
				rlp->setLoginFailure(false);
			} 
			else
			{
				Log::addLog(Log::LogType::LOG_INFO, 4, lp->getUsername() + " failed to login!");
				rlp->setLoginFailure(true);
			}

			_user->addMsgToMsgQueue(rlp);
			

			break;
		}

	case msgBase::MsgType::FRIENDLISTREQUEST:
		{
			//FriendlistResponse::ptr rfl = FriendlistResponse::ptr(new FriendlistResponse());

			//for (User::ptr user : users.baseSet)
			//{
			//	TODO: redo friends list request
			//	std::pair<std::string, boost::uuids::uuid> tmpPair(user->getUserData().userName, user->getUserData().uuid);
			//	rfl->addToFriendList(tmpPair);
			//}

			//_user->addMsgToMsgQueue(rfl);

			break;
		}

	case msgBase::MsgType::CREATEUSERREQUEST:
		{
			CreateUserRequest::ptr cu = boost::static_pointer_cast<CreateUserRequest>(p);
			CreateUserResponse::ptr rcu = CreateUserResponse::ptr(new CreateUserResponse());


			long res = sqlManager.createUser(cu->getUserName(), cu->getUserPassword());

			if (res != -1)
			{
				Log::addLog(Log::LogType::LOG_INFO, 3, "User created with userID: " + std::to_string(res));
				_user->setUserID(res);
				_user->setUserStatus(User::UserStatus::USER);
				rcu->setCreateFailure(false);
			} 
			else
			{
				Log::addLog(Log::LogType::LOG_ERROR, 1, "Failed to create user with username: " + cu->getUserName());
				rcu->setCreateFailure(true);
			}

			_user->addMsgToMsgQueue(rcu);

			break;
		}

	case msgBase::MsgType::LOGOUTREQUEST:
		{
			Log::addLog(Log::LogType::LOG_INFO, 4, "UserID: " + std::to_string(_user->getUserID()) + " has disconnected!");
			_user->disconnect();
			users.erase(_user);			
			break;
		}

	case msgBase::MsgType::GAMEMESSAGE:
		{
			GameMessage::ptr gmp = boost::static_pointer_cast<GameMessage>(p);
			GameMaster::getInstance().handleGameMessage(gmp);
			break;
		}
	default:
		Log::addLog(Log::LogType::LOG_ERROR, 0,"Received packet that are not yet implemented: " + p->getType());
		break;
	}
}

void UserManager::startIO()
{
	ioThread = std::thread(boost::bind(&UserManager::startIOPrivate, shared_from_this()));
}

void UserManager::startIOPrivate()
{
	try
	{
		ioService->run();

		Log::addLog(Log::LogType::LOG_DEBUG, 3,"IO services stopped");
	}
	catch (const boost::system::error_code& err)
	{
		Log::addLog(Log::LogType::LOG_ERROR, 2, err.message());
	}
	catch (...)
	{
		Log::addLog(Log::LogType::LOG_ERROR, 0, "Uncaught exception in IO thread. Fix!");
	}
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

#include "stdafx.h"
#include "UserManager.h"
#include "GameMaster.h"

#include <Chat.h>
#include <LoginRequest.h>
#include <FriendlistResponse.h>
#include <FriendlistRequest.h>
#include <CreateUserRequest.h>
#include <AcknowledgeLast.h>
#include <GameMessage.h>
#include <stdexcept>

static const int loginAtemptesAlowed = 3;
static const int hammerTime = 30;

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
	acceptor->close();
	ioService->stop();
}

void UserManager::listenForNewClientConnections()
{
	boost::shared_ptr<tcp::socket> socket(new tcp::socket(*ioService));

	acceptor->async_accept(*socket, boost::bind(&UserManager::handleIncomingClient, this, socket, boost::asio::placeholders::error));
}

void UserManager::handleIncomingClient( boost::shared_ptr<tcp::socket> _soc, const boost::system::error_code& _errorCode )
{
	//if (checkIfIPAlowed(_soc->remote_endpoint().address()))
	//{
		User::ptr u = User::ptr(new User(_soc));
		users.insert(u);
		u->listen();
		std::string ip = _soc->remote_endpoint().address().to_string();
		Log::addLog(Log::LogType::LOG_INFO, 4, "New connection established from: " + ip, __FILE__, __LINE__);
		listenForNewClientConnections();
	//} 
	//else
	//{
	//}
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
		Log::addLog(Log::LogType::LOG_ERROR, 0, "Received unregistered packet: " + (int)_header.type, __FILE__, __LINE__);
		return;
	}

	switch (_header.type)
	{

	case msgBase::MsgType::CHAT:
		{
			if (_user->getUserType() == User::UserType::UNVERIFIED)
			{
				_user->disconnect();
				users.erase(_user);	
				break;
			}

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
		}

	case msgBase::MsgType::LOGINREQUEST:
		{
			if (_user->getUserState() != User::UserState::UNKNOWN)
			{
				_user->disconnect();
				users.erase(_user);	
				break;
			}

			LoginRequest::ptr lp = boost::static_pointer_cast<LoginRequest>(p);

			long res = sqlManager.verifyUser(lp->getUsername(), lp->getPassword());

			if (res != -1 && !userAllredyLogedin(res))
			{
				_user->setUserID(res);
				_user->setUserType(User::UserType::USER);
				_user->setUserState(User::UserState::AVAILABLE);
				Log::addLog(Log::LogType::LOG_INFO, 4, "Username: " + lp->getUsername() + " Logged in", __FILE__, __LINE__);

				AcknowledgeLast::ptr ack = AcknowledgeLast::ptr(new AcknowledgeLast());
				ack->setAck(msgBase::MsgType::LOGINREQUEST, false);

				_user->addMsgToMsgQueue(ack);
			} 
			else
			{
				//Log::addLog(Log::LogType::LOG_INFO, 4, lp->getUsername() + " failed to login!", __FILE__, __LINE__);
				//addIPtoBlackList(_user->getSocket()->remote_endpoint().address());

				_user->disconnect();
				users.erase(_user);	
			}

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
			if (_user->getUserState() != User::UserState::UNKNOWN)
			{
				_user->disconnect();
				users.erase(_user);	
				break;
			}

			CreateUserRequest::ptr cu = boost::static_pointer_cast<CreateUserRequest>(p);
			AcknowledgeLast::ptr ack = AcknowledgeLast::ptr(new AcknowledgeLast());


			long res = sqlManager.createUser(cu->getUserName(), cu->getUserPassword());

			if (res != -1)
			{
				Log::addLog(Log::LogType::LOG_INFO, 3, "User created with userID: " + std::to_string(res), __FILE__, __LINE__);
				_user->setUserID(res);
				_user->setUserType(User::UserType::USER);
				_user->setUserState(User::UserState::AVAILABLE);
				ack->setAck(msgBase::MsgType::CREATEUSERREQUEST, false);
			} 
			else
			{
				Log::addLog(Log::LogType::LOG_ERROR, 1, "Failed to create user with username: " + cu->getUserName(), __FILE__, __LINE__);
				ack->setAck(msgBase::MsgType::CREATEUSERREQUEST, true);
			}

			_user->addMsgToMsgQueue(ack);

			break;
		}

	case msgBase::MsgType::LOGOUTREQUEST:
		{
			Log::addLog(Log::LogType::LOG_INFO, 4, "UserID: " + std::to_string(_user->getUserID()) + " has disconnected!", __FILE__, __LINE__);
			_user->disconnect();
			users.erase(_user);			
			break;
		}

	case msgBase::MsgType::GAMEMESSAGE:
		{
			if (_user->getUserType() != User::UserType::UNVERIFIED && (_user->getUserState() == User::UserState::AVAILABLE || _user->getUserState() == User::UserState::INGAME))
			{
				GameMessage::ptr gmp = boost::static_pointer_cast<GameMessage>(p);

				if ((_user->getUserState() != User::UserState::AVAILABLE) == (gmp->getGameType() == GameMessage::GameMsgType::CREATEGAMEREQUEST))
				{
					Log::addLog(Log::LogType::LOG_INFO, 4, "UserID: " + std::to_string(_user->getUserID()) + " has tried to execute unauthorized command, user disconnected!", __FILE__, __LINE__);
					_user->disconnect();
					users.erase(_user);			
					break;
				}

				GameMaster::getInstance().handleGameMessage(gmp, _user);
				break;
			} 
			else
			{
				Log::addLog(Log::LogType::LOG_INFO, 4, "UserID: " + std::to_string(_user->getUserID()) + " has tried to execute unauthorized command, user disconnected! ## Line: ", __FILE__, __LINE__);
				_user->disconnect();
				users.erase(_user);
				break;
			}			
		}
	default:
		Log::addLog(Log::LogType::LOG_ERROR, 0,"Received packet that are not yet implemented: " + p->getType(), __FILE__, __LINE__);
		_user->disconnect();
		users.erase(_user);
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

		Log::addLog(Log::LogType::LOG_INFO, 3,"IO services stopped", __FILE__, __LINE__);
	}
	catch (const boost::system::error_code& err)
	{
		Log::addLog(Log::LogType::LOG_ERROR, 2, err.message(), __FILE__, __LINE__);
	}
	catch (...)
	{
		Log::addLog(Log::LogType::LOG_ERROR, 0, "Uncaught exception in IO thread. Fix!", __FILE__, __LINE__);
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

bool UserManager::userAllredyLogedin( long _uid )
{
	for (User::ptr u : users.baseSet)
	{
		if (_uid == u->getUserID())
		{
			return true;
		}
	}

	return false;
}

bool UserManager::checkIfIPAlowed( boost::asio::ip::address _addr )
{
	if (ipBlackList.count(_addr) > 0)
	{
		userIP uip = ipBlackList.at(_addr);
		boost::posix_time::ptime pt = boost::posix_time::second_clock::local_time();

		if ((pt - uip.firstConnect).total_seconds() < hammerTime && uip.connectAtempts >= loginAtemptesAlowed) //user last login attempt time are within hammer time and attempt count are higher then allowed count. 
		{
			return false;
		}
		else if ((pt - uip.firstConnect).total_seconds() > hammerTime) // user hammer timer has expired
		{
			ipBlackList.erase(_addr);
			return true;

		}
	}
	return true;
}

void UserManager::addIPtoBlackList( boost::asio::ip::address _addr )
{
	if (ipBlackList.count(_addr) > 0)
	{
		ipBlackList.at(_addr).connectAtempts++;
		return;
	}

	userIP newUser;
	newUser.connectAtempts = 1;
	newUser.firstConnect = boost::posix_time::second_clock::local_time();

	ipBlackList.insert(std::make_pair(_addr, newUser));

	return;
}

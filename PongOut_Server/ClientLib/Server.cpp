#include "stdafx.h"
#include "Server.h"

Server::Server(const std::string _ipAdress, std::uint16_t _port)
	: addr(_ipAdress),
	port(_port),
	soc(io)
{
	 PacketHandler::getInstance().initRegister();
}


Server::~Server(void)
{
}

void Server::connect()
{
	if (soc.is_open())
	{
		soc.shutdown(boost::asio::socket_base::shutdown_both);
		soc.close();
	}

	tcp::resolver res(io);
	std::stringstream ss;
	ss << port;
	tcp::resolver::query q(addr, ss.str());
	tcp::resolver::iterator resIt = res.resolve(q);
	try
	{
		
		boost::asio::connect(soc, resIt);
	}
	catch (boost::system::system_error&)
	{
		messages.push(message(msgBase::MsgType::INTERNALMESSAGE, "Failed to connect to server!"));
		return;
	}

	listen();

	if (ioThread.joinable())
	{
		messages.push(message(msgBase::MsgType::INTERNALMESSAGE, "Waiting for previous connection to end..."));
		ioThread.join();
	}

	ioThread = std::thread(boost::bind(&Server::startIO, shared_from_this()));
}

void Server::login( const std::string& _userName, const std::string& _password )
{
	LoginRequest::ptr lp = LoginRequest::ptr(new LoginRequest());
	lp->setLogin(_userName, _password);
	write(lp);
}

void Server::logout()
{
	LogoutRequest::ptr p = LogoutRequest::ptr(new LogoutRequest());
	write(p);
	soc.close();
	ioThread.join();
}

void Server::write( msgBase::ptr _msg )
{
	if (soc.is_open())
	{
		msgWriteBufffer = _msg->getData();
		boost::asio::async_write(soc, boost::asio::buffer(msgWriteBufffer), boost::bind(&Server::handleWrite, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	}
}

void Server::handleWrite( const boost::system::error_code& _err, size_t _byte )
{
	if (_err)
	{
		messages.push(message(msgBase::MsgType::INTERNALMESSAGE, _err.message()));
	}	
}

void Server::listen()
{
	if (soc.is_open())
	{
		soc.async_read_some(boost::asio::buffer(msgListenBuffer), boost::bind(&Server::handleIncomingMessage, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	}
}

void Server::handleIncomingMessage(const boost::system::error_code& _error, size_t _bytesTransferred)
{
	if (_error == boost::asio::error::eof)
	{
		messages.push(message(msgBase::MsgType::INTERNALMESSAGE, "disconnected by server"));
		return;
	} 
	else if (_error)
	{
		messages.push(message(msgBase::MsgType::INTERNALMESSAGE, _error.message()));
		return;
	}

	int readChars = 0;

	for (ever)
	{
		if (fullMsgBuffer.size() < sizeof(msgBase::header))
		{
			unsigned int missingChars = sizeof(msgBase::header) - fullMsgBuffer.size();

			if ((_bytesTransferred - readChars) >= missingChars)
			{
				for (unsigned int i = 0; i < missingChars; i++)
				{
					fullMsgBuffer.push_back(msgListenBuffer[readChars + i]);
				}
				readChars += missingChars;
				head = PacketHandler::getInstance().getMeassageHeader(fullMsgBuffer);			
			}
			else
			{
				for (unsigned int i = 0; i < (_bytesTransferred - readChars); i++)
				{
					fullMsgBuffer.push_back(msgListenBuffer[readChars + i]);
				}
				listen();
				return;
			}
		}

		unsigned int missingChars = head.length - (fullMsgBuffer.size() - sizeof(msgBase::header));

		if ((_bytesTransferred - readChars) >= missingChars)
		{
			for (unsigned int i = 0; i < missingChars; i++)
			{
				fullMsgBuffer.push_back(msgListenBuffer[readChars + i]);
			}
			readChars += missingChars;
			messageActionSwitch(head, fullMsgBuffer);
			fullMsgBuffer.clear();
		} 
		else
		{
			for (unsigned int i = 0; i < (_bytesTransferred - readChars); i++)
			{
				fullMsgBuffer.push_back(msgListenBuffer[readChars + i]);
			}
			listen();
			return;
		}
	}
}

void Server::messageActionSwitch( const msgBase::header& _header, const std::deque<char>& _meassage )
{
	msgBase::ptr p = PacketHandler::getInstance().interpretMessage(_header.type, _meassage);

	switch (_header.type)
	{
	case msgBase::MsgType::CHAT:
		{
			messages.push(message(msgBase::MsgType::CHAT, p));
			break;
		}

	case msgBase::MsgType::FRIENDLISTRESPONSE:
		{
			messages.push(message(msgBase::MsgType::FRIENDLISTRESPONSE, p));
			break;
		}

	case msgBase::MsgType::ACKNOWLEDGELAST:
		{
			messages.push(message(msgBase::MsgType::ACKNOWLEDGELAST, p));
			break;
		}

	case msgBase::MsgType::GAMEMESSAGE:
		{
			GameMessage::ptr gmp = boost::static_pointer_cast<GameMessage>(p);		
			GameHandler::getInstance().handleGameMessage(gmp, messages);
			break;
		}

	default:
		messages.push(message(p->getHeader().type, p));
		break;
	}
}

void Server::requestFriends()
{
	FriendlistRequest::ptr rf = FriendlistRequest::ptr(new FriendlistRequest());
	write(rf);
}

void Server::createAccount( std::string _userName, std::string _userPassword )
{
	CreateUserRequest::ptr rcu = CreateUserRequest::ptr(new CreateUserRequest());
	rcu->setCredentials(_userName, _userPassword);
	write(rcu);
}

void Server::startIO()
{
	io.run();
	std::cout << "IO thread done." << std::endl;
}

//void Server::sendChatMsg( std::string _name, std::string _msg )
//{
//	Chat::ptr cp = Chat::ptr(new Chat());
//
//	for (auto a : friends)
//	{
//		if (a.first == _name)
//		{
//			cp->setMsg(_msg, a.second, userName);
//			write(cp);
//
//			return;
//		}
//	}
//
//	std::cout << "Friend \"" << _name << "\" does not exist." << std::endl;
//}

Server::message Server::getNextMessage()
{
	return messages.pop();
}

int Server::getMsgQueueSize()
{
	return messages.baseQueue.size();
}

void Server::createGame( int _mapID, int _ballSpeed, int _paddleSpeed, int _suddenDeathTime, char _fogOfWar, char _powerUps )
{
	CreateGameRequest::ptr cgp = CreateGameRequest::ptr(new CreateGameRequest());
	CommonTypes::GameInitInfo info;

	info.ballSpeed = _ballSpeed;
	info.mapID = _mapID;
	info.paddleSpeed = _paddleSpeed;
	info.suddenDeathTime = _suddenDeathTime;
	info.fogOfWar = _fogOfWar;
	info.powerUps = _powerUps;

	cgp->setGameParam(info);
	write(cgp);
}

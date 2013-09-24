#include "stdafx.h"
#include "Server.h"
#include <queue>


Server::Server(const std::string _ipAdress, std::uint16_t _port) : addr(_ipAdress), port(_port), soc(io)
{
	 PacketHandler::getInstance().initRegister();
}


Server::~Server(void)
{
}

void Server::connect(const std::string& _userName, const std::string& _password)
{
	tcp::resolver res(io);
	std::stringstream ss;
	ss << port;
	tcp::resolver::query q(addr, ss.str());
	tcp::resolver::iterator resIt = res.resolve(q);
	try
	{
		boost::asio::connect(soc, resIt);
	}
	catch (boost::system::system_error& e)
	{
		std::cout << "Failed to connect to server." << std::endl;
		std::cout << e.what() << std::endl;
		return;
	}

	Login::ptr lp = Login::ptr(new Login());
	lp->setLogin(_userName, _password);
	write(lp);
	listen();
	ioThread = std::thread(boost::bind(&Server::startIO, shared_from_this()));
}

void Server::write( msgBase::ptr _msg )
{
	msgWriteBufffer = _msg->getData();
	boost::asio::async_write(soc, boost::asio::buffer(msgWriteBufffer), boost::bind(&Server::handleWrite, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void Server::handleWrite( const boost::system::error_code& _err, size_t _byte )
{
	if (_err)
	{
		std::cerr << _err.message() << std::endl;
	}	
}

void Server::listen()
{
	soc.async_read_some(boost::asio::buffer(msgListenBuffer), boost::bind(&Server::handleIncomingMessage, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void Server::handleIncomingMessage(const boost::system::error_code& _error, size_t _bytesTransferred)
{
	if (_error == boost::asio::error::eof)
	{
		return;
	} 
	else if (_error)
	{
		throw boost::system::error_code(_error);
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
			Chat::ptr cp = boost::static_pointer_cast<Chat>(p);
			std::cout << cp->getMsg() << std::endl;
			break;
			//TODO: Call ClientEventLib chat event here when done!!
		}

	case msgBase::MsgType::RESPONSEFRIENDLIST:
		{
			ResponseFriendlist::ptr rfl = boost::static_pointer_cast<ResponseFriendlist>(p);
			friends = rfl->getAllFriends();
			std::cout << "Friends list received:" << std::endl;

			for (auto a : friends)
			{
				std::cout << a.first << "  :  " << a.second << std::endl;
			}

			break;
		}

	default:
		std::cerr << "Received unknown packet: " << (int)p->getHeader().type << std::endl;
		break;
	}
}

void Server::requestFriends()
{
	RequestFriendlist::ptr rf = RequestFriendlist::ptr(new RequestFriendlist());
	write(rf);
}

void Server::startIO()
{
	io.run();
}
#include "stdafx.h"
#include "Server.h"


Server::Server(const std::string _ipAdress, std::uint16_t _port) : addr(_ipAdress), port(_port), soc(io)
{
	handler.initRegister();
}


Server::~Server(void)
{
}

void Server::connect()
{
	tcp::resolver res(io);
	std::stringstream ss;
	ss << port;
	tcp::resolver::query q(addr, ss.str());
	resIt = res.resolve(q);
	boost::asio::connect(soc, resIt);
	startListen();
	ioThread = std::thread(boost::bind(&Server::startIO, shared_from_this()));
}

void Server::startListen()
{
	soc.async_read_some(boost::asio::buffer(msgBuffer), boost::bind(&Server::handleMessage, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void Server::handleMessage(const boost::system::error_code& _error, size_t _bytesTransferred)
{
	if (_error == boost::asio::error::eof)
	{
		return;
	} 
	else if (_error)
	{
		throw boost::system::error_code(_error);
	}

	std::vector<char> fullMeassage(msgBuffer.begin(), msgBuffer.end());
	msgBuffer.assign(0);	
	msgBase::header head = handler.getMeassageHeader(fullMeassage);
	messageActionSwitch(head, fullMeassage);
	startListen();
}

void Server::messageActionSwitch( const msgBase::header& _header, const std::vector<char>& _meassage )
{
	msgBase::ptr p = handler.interpretMessage(_header.type, _meassage);

	switch (_header.type)
	{
	case msgBase::MsgType::CHAT:
		{
			Chat::ptr cp = boost::static_pointer_cast<Chat>(p);
			std::cout << cp->getMsg() << std::endl;
			break;
			//TODO: Call ClientEventLib chat event here when done!!
		}
	default:
		std::cerr << "Received unknown packet: " << (int)p->getHeader().type << std::endl;
		break;
	}
}

void Server::startIO()
{
	io.run();
}
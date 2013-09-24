#include "stdafx.h"
#include "Client.h"
#include "Chat.h"
#include <boost/shared_ptr.hpp>
#include <boost/uuid/uuid_io.hpp>

using boost::asio::ip::tcp;

Client::Client(const std::string _addr)
{
	handler.initRegister();
	doShit(_addr);
}


Client::~Client(void)
{
}

void Client::doShit( const std::string _addr ) //OBS SKRÄP KOD!!!!
{
	try
	{
		boost::asio::io_service io;
		tcp::resolver res(io);
		tcp::resolver::query q( _addr, "65000");
		tcp::resolver::iterator end = res.resolve(q);

		tcp::socket soc(io);
		boost::asio::connect(soc, end);

		for (ever)
		{
			boost::array<char, 128> buf;
			boost::system::error_code err;

			size_t len = soc.read_some(boost::asio::buffer(buf), err);

			if ( err == boost::asio::error::eof)
			{
				break;
			} 
			else if (err)
			{
				throw boost::system::error_code(err);
			}

			std::vector<char> tmp(buf.begin(), buf.end());

			msgBase::ptr p = handler.interpretMessage(msgBase::MsgType::CHAT, tmp);

			switch (p->getHeader().type)
			{
			case msgBase::MsgType::CHAT:
				{
					Chat::ptr cp = boost::static_pointer_cast<Chat>(p);
					std::cout << cp->getMsg() << std::endl;
					std::cout << cp->getUUID() << std::endl;
					break;
				}
			default:
				std::cerr << "Recieved unkown packet: " << (int)p->getHeader().type << std::endl;
				break;
			}	
		}
	}
	catch (std::exception& e)
	{
		std::cerr << "Caught exception" << std::endl;
		std::cerr << e.what() << std::endl;
	}
}

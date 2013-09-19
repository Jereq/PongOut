#include "stdafx.h"
#include "UserManager.h"
#include <Chat.h>

UserManager::UserManager(boost::asio::io_service* _io) 
			: ioService(_io), 
			acceptor(*ioService, tcp::endpoint(tcp::v4(), 65000))
{
}


UserManager::~UserManager(void)
{
}

void UserManager::listen()
{
	tcp::socket* socket = new tcp::socket(*ioService);

	acceptor.async_accept(*socket, boost::bind(&UserManager::handleAccept, this, socket, boost::asio::placeholders::error));
}

void UserManager::handleAccept( tcp::socket* _soc, const boost::system::error_code& _errorCode )
{
	User::ptr u = User::ptr(new User(_soc));
	users.insert(u);
	Chat::ptr c = Chat::ptr(new Chat());

	c->setMsg("tittut!", boost::uuids::uuid());
	u->sendMsg(c);
	std::cout << "New user connected" << std::endl;
	listen();
}

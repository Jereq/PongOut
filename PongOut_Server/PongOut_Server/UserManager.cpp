#include "stdafx.h"
#include "UserManager.h"
#include <Chat.h>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

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
	msgBase::userData ud;
	ud.userName = "noName";
	ud.uuid = boost::uuids::random_generator()();

	User::ptr u = User::ptr(new User(_soc, ud));
	users.insert(u);
	Chat::ptr c = Chat::ptr(new Chat());

	c->setMsg("The server says: HI!!", boost::uuids::uuid());
	u->addMsgToMsgQueue(c);
	std::cout << "New user connected and given ID: " << ud.uuid << std::endl;
	listen();
}

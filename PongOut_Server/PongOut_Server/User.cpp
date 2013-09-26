#include "stdafx.h"
#include "User.h"

#include "UserManager.h"

User::User(boost::shared_ptr<tcp::socket> _socket, boost::uuids::uuid _uuid)
	: socket(_socket)
{
	userData.uuid = _uuid;
}


User::~User(void)
{
	int dummy = 42;
}

boost::shared_ptr<tcp::socket> User::getSocket()
{
	return socket;
}

void User::handleWrite( const boost::system::error_code& _err, size_t _byte )
{
	std::lock_guard<std::mutex> lock(msgBufferLock);
	msgWriteBufferQueue.pop();

	if (!msgWriteBufferQueue.empty())
	{
		sendMsg();
	}

	std::cerr << _err.message() << std::endl;
}

void User::sendMsg()
{
	msgWriteBuffer = msgWriteBufferQueue.front()->getData();
	boost::asio::async_write(*socket, boost::asio::buffer(msgWriteBuffer), boost::bind(&User::handleWrite, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void User::addMsgToMsgQueue(const msgBase::ptr& _msgPtr )
{
	std::lock_guard<std::mutex> lock(msgBufferLock);
	msgWriteBufferQueue.push(_msgPtr);
	if (msgWriteBufferQueue.size() == 1)
	{
		sendMsg();
	}
}

void User::listen()
{
	socket->async_read_some(boost::asio::buffer(msgListenBuffer), boost::bind(&User::handleIncomingMeassage, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void User::handleIncomingMeassage( const boost::system::error_code& _error, size_t _bytesTransferred )
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
			UserManager::getInstance()->messageActionSwitch(head, fullMsgBuffer, shared_from_this());
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

msgBase::userData User::getUserData()
{
	return userData;
}

void User::setUserNamePass(const std::string& _name, const std::string& _pass)
{
	userData.userName = _name;
	userData.password = _pass;
}

void User::disconnect()
{
	socket->shutdown(boost::asio::socket_base::shutdown_both);
	socket->close();
}

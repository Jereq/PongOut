#include "stdafx.h"
#include "User.h"

#include "UserManager.h"

User::User(boost::shared_ptr<tcp::socket> _socket)
	: socket(_socket), status(UserStatus::UNVERIFIED)
{
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

	if (!_err)
	{
		Log::addLog(Log::LogType::LOG_ERROR, 4, _err.message());
	} 

	//TODO: handle whats happens case off error

	if (!msgWriteBufferQueue.empty())
	{
		sendMsg();
	}
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
	if (socket->is_open())
	{
		socket->async_read_some(boost::asio::buffer(msgListenBuffer), boost::bind(&User::handleIncomingMeassage, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	}
}

void User::handleIncomingMeassage( const boost::system::error_code& _error, size_t _bytesTransferred )
{
	if (_error == boost::asio::error::eof)
	{
		UserManager::getInstance()->users.erase(shared_from_this());
		Log::addLog(Log::LogType::LOG_INFO, 1, "User " + std::to_string(getUserID()) + " disconnected: end of stream");
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

unsigned int User::getUserID()
{
	return id;
}

void User::disconnect()
{
	socket->shutdown(boost::asio::socket_base::shutdown_both);
	socket->close();
}

void User::setUserStatus( UserStatus _status )
{
	status = _status;
}

void User::setUserID( unsigned int _id )
{
	id = _id;
}

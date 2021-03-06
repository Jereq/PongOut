#include "stdafx.h"
#include "User.h"

#include "UserManager.h"

User::User(boost::shared_ptr<tcp::socket> _socket) : socket(_socket), userType(UserType::UNVERIFIED), userState(UserState::UNKNOWN)
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

	if (_err)
	{
		Log::addLog(Log::LogType::LOG_ERROR, 4, _err.message(), __FILE__, __LINE__); //TODO: solve funky shit! ERROR : The file handle supplied is not valid
		disconnect();
	}
	else if (!msgWriteBufferQueue.empty())
	{
		sendMsg();
	}
}

void User::sendMsg()
{
	auto& packet = msgWriteBufferQueue.front();
	msgWriteBuffer = packet->getData();
	if (msgWriteBuffer.size() != packet->getHeader().length + sizeof(msgBase::header))
	{
		Log::addLog(Log::LogType::LOG_ERROR, 1, "Packet (" + packet->getType() + ") has incorrect size: " + std::to_string(msgWriteBuffer.size()) + " is not " + std::to_string(packet->getHeader().length + sizeof(msgBase::header)), __FILE__, __LINE__);
		return;
	}
	boost::asio::async_write(*socket, boost::asio::buffer(msgWriteBuffer), boost::bind(&User::handleWrite, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void User::addMsgToMsgQueue(const msgBase::ptr& _msgPtr )
{
	std::lock_guard<std::mutex> lock(msgBufferLock);

	if (msgWriteBufferQueue.size() > 100)
	{
		Log::addLog(Log::LogType::LOG_ERROR, 1, "User: " + std::to_string(id) + "has to many messages in msgQ", __FILE__, __LINE__);
		return;
	}

	//Log::addLog(Log::LogType::LOG_INFO, 1, "User: " + std::to_string(id) + " has:\t" + std::to_string(msgWriteBufferQueue.size()) + " in msgQ", __FILE__, __LINE__);

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
		disconnect();
		UserManager::getInstance()->users.erase(shared_from_this());
		Log::addLog(Log::LogType::LOG_INFO, 1, "User " + std::to_string(getUserID()) + " disconnected: end of stream", __FILE__, __LINE__);
		return;
	} 
	else if (_error)
	{
		disconnect();
		UserManager::getInstance()->users.erase(shared_from_this());
		Log::addLog(Log::LogType::LOG_INFO, 1, "User " + std::to_string(getUserID()) + " Aborted connection", __FILE__, __LINE__);
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
	if (socket && socket->is_open())
	{
		socket->shutdown(boost::asio::socket_base::shutdown_both);
		socket->close();
	}
}

void User::setUserType( UserType _type )
{
	userType = _type;
}

void User::setUserID( unsigned int _id )
{
	id = _id;
}

User::UserType User::getUserType()
{
	return userType;
}

int User::getRefereeID()
{
	return refID;
}

void User::setReffereeID( int _refID )
{
	refID = _refID;
}

User::UserState User::getUserState()
{
	return userState;
}

void User::setUserState( UserState _state )
{
	userState = _state;
}

#pragma once

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <PacketHandler.h>

class Client
{
public:
	Client(const std::string _addr);

	void doShit( const std::string _addr );

	~Client(void);

	PacketHandler handler;
};


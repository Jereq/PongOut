// SimpleClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Server.h>

int _tmain(int argc, _TCHAR* argv[])
{
	Server::ptr s(new Server("127.0.0.1", 65000));
	s->connect();

	for (ever)
	{
	}

	system("pause");
	return 0;
}


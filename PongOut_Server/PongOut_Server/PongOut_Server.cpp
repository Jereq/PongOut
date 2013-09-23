// PongOut_Server.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include "UserManager.h"
#include <boost/asio.hpp>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{	
	cout << "PongOut server started!" << endl;

	UserManager::getInstance()->listenForNewClientConnections();
	UserManager::getInstance()->startIO();

	for (ever)
	{
	}

	//system("pause");
	return 0;
}


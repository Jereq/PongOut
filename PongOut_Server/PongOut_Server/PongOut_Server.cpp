// PongOut_Server.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include "UserManager.h"
#include <boost/asio.hpp>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	boost::asio::io_service* io = new boost::asio::io_service();
	UserManager man(io);
	
	cout << "PongOut server started!" << endl;

	man.listen();
	io->run();

	system("pause");
	return 0;
}


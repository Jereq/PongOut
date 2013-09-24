// SimpleClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Server.h>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	Server::ptr s(new Server("127.0.0.1", 6500));

	string command, userName, password;

	for (ever)
	{
		cout << "======== Commands list ========" << endl 
			 << "# Connect \t : \t Provide login information and connect to server" << endl 
			 << "# Request \t : \t Requests friends list from server" << endl 
			 << "# Chat \t\t : \t Chat with friend [friendName] [message]" << endl;
		cin >> command;

		if (command == "Connect")
		{
			cout << "Provide username: ";
			cin >> userName;
			cout << "Provide password: ";
			cin >> password;

			s->connect(userName, password);	
		}
		else if (command == "Request")
		{
			s->requestFriends();
		}
		else if (command == "Chat")
		{
		} 
	}

	system("pause");
	return 0;
}


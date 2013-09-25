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
			 << "# Chat \t\t : \t Chat with friends" << endl;
		cin >> command;

		if (command == "Connect")
		{
			cout << "Provide username: ";
			cin >> userName;
			cout << "Provide password: ";
			cin >> password;

			s->connect(userName, password);	
		}
		else if (command == "Chat")
		{
			system("CLS");
			s->requestFriends();
			string toUser, msg;
			cout << endl << "Type the name of the friend you want to chat with" << endl;
			cin >> toUser;
			cout << "##Chatting with: " << toUser << endl;

			for (ever)
			{
				cin >> msg;
				s->sendChatMsg(toUser, msg);
			}
		} 
	}

	system("pause");
	return 0;
}


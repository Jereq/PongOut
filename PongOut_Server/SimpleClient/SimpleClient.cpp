// SimpleClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Server.h>
#include <chrono>

using namespace std;

void waitForMsg(Server::ptr _s, int _timeToWait)
{
	auto start = chrono::high_resolution_clock::now();

	while (_s->getMsgQueueSize() < 1)
	{
		chrono::milliseconds pause(20);
		this_thread::sleep_for(pause);

		auto current = chrono::high_resolution_clock::now();

		if (chrono::duration_cast<chrono::seconds>(current - start).count() > _timeToWait)
		{
			cerr << "Server not responding!" << endl;
			return;
		}
	}

	for (int i = 0; i < _s->getMsgQueueSize(); i++)
	{
		Server::message tmp = _s->getNextMessage();

		switch (tmp.type)
		{
		case msgBase::MsgType::FRIENDLISTRESPONSE:
			{
				FriendlistResponse::ptr rfl = boost::static_pointer_cast<FriendlistResponse>(tmp.msg);
				vector<pair<string, unsigned int>> friends = rfl->getAllFriends();

				cout << "Friends list received:" << endl;
				for (auto a : friends)
				{
					cout << a.first << "  :  " << a.second << endl;
				}
				break;
			}
		case msgBase::MsgType::ACKNOWLEDGELAST:
			{
				AcknowledgeLast::ptr rc = boost::static_pointer_cast<AcknowledgeLast>(tmp.msg);

				//TODO: fix AcknowledgeLast on client side

				cout << "Connected successfully!" << endl;
				break;
			}
		case  msgBase::MsgType::INTERNALMESSAGE:
			{
				cout << tmp.strMsg << endl;
				break;
			}
		}
	}
}



int _tmain(int argc, _TCHAR* argv[])
{
	Server::ptr s(new Server("127.0.0.1", 6500));

	string command, userName, password;
	bool firstRun = true;

	s->connect();

	for (ever)
	{
		system("CLS");

		if (!firstRun)
		{
			waitForMsg(s, 5);
		}

		firstRun = false;
		
		cout << "======== Commands list ========" << endl 
			 //<< "# Connect \t : \t Provide login information and connect to server" << endl  
			 << "# Create \t\t : \t Create account" << endl
			 << "# Login \t\t : \t Login to existing account" << endl
			 << "# Logout \t\t : \t Logout from account" << endl
			 << "# Game \t\t : \t Create new game" << endl;
		cin >> command;

		//if (command == "Connect" || command == "connect")
		//{
		//	s->connect();			
		//}
		if (command == "Login" || command == "login")
		{
			system("CLS");
			cout << "Provide username: ";
			cin >> userName;
			cout << "Provide password: ";
			cin >> password;

			s->login(userName, password);
		}
		else if (command == "Logout" || command == "logout")
		{
			system("CLS");
			s->logout();
		}
		else if (command == "Create" || command == "create")
		{
			system("CLS");
			cout << "Provide new username: ";
			cin >> userName;
			cout << "Provide new password: ";
			cin >> password;
			s->createAccount(userName, password);
		}
		else if (command == "Game" || command == "game")
		{
			s->createGame(0, 10, 10, 120, true, true);
		}
	}

	system("pause");
	return 0;
}


//else if (command == "Chat" || command == "chat")
//{
//	system("CLS");
//	s->requestFriends();
//	string toUser, msg;
//	cout << endl << "Type the name of the friend you want to chat with" << endl;
//	cin >> toUser;
//	cout << "##Chatting with: " << toUser << endl;

//	for (ever)
//	{
//		getline(cin, msg);
//		s->sendChatMsg(toUser, msg);
//	}
//} 


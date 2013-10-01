// PongOut_Server.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include "UserManager.h"
#include <boost/asio.hpp>
#include <condition_variable>

using namespace std;
bool quit = false;
std::mutex closeMutex;
std::condition_variable close;

BOOL WINAPI ConsoleHandler(DWORD CEvent)//BAD WINDOWS!! MISSBEHAVING WINDOWS!!
{
	switch (CEvent)
	{
	case CTRL_C_EVENT:
	case CTRL_BREAK_EVENT:
			quit = true;
		break;

	case CTRL_CLOSE_EVENT:
	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
		quit = true;

		std::unique_lock<std::mutex> closeLock(closeMutex);
		close.wait(closeLock);
		break;
	}

	return TRUE;
}

int _tmain(int argc, _TCHAR* argv[])
{
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler,TRUE);

	Log::addLog(Log::LogType::LOG_INFO, "PongOut server started!");

	UserManager::getInstance()->listenForNewClientConnections();
	UserManager::getInstance()->startIO();

	while (!quit)
	{
	}

	UserManager::getInstance()->destroy();
	Log::destroy();

	close.notify_all();
	//system("pause");
	return 0;
}


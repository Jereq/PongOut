#include <iostream>
#include <sstream>

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include "PongOutConfig.h"
#include "Game.h"
#include "CoreSystemWindows.h"

Game* game;
HWND  hwnd;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	static char szAppName[] = "PongOut";

	CoreSystemWindows* iCore = new CoreSystemWindows();
	
	WNDCLASSEX  wndClass;

	wndClass.cbSize			= sizeof(wndClass);
	wndClass.style			= CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc	= iCore->WndProc;
	wndClass.cbClsExtra		= 0;
	wndClass.cbWndExtra		= 0;
	wndClass.hInstance		= hInstance;
	wndClass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hIconSm		= LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground	= (HBRUSH) GetStockObject(WHITE_BRUSH);
	wndClass.lpszClassName	= szAppName;
	wndClass.lpszMenuName	= NULL;

	RegisterClassEx(&wndClass);

	hwnd = CreateWindow(szAppName, "PongOut",
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT, CW_USEDEFAULT,
				CW_USEDEFAULT, CW_USEDEFAULT,
				NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	iCore->setHwnd(hwnd);

	game = new Game((ICoreSystem*)iCore);
	game->run();

	delete game;
	game = NULL;

	return 0;
}

std::string getVersionString()
{
	std::ostringstream stream;
	stream << "PongOut " << PongOut_VERSION_MAJOR << "." << PongOut_VERSION_MINOR << "." << PongOut_VERSION_PATCH;

	return stream.str();
}
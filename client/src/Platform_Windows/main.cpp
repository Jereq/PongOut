#include <iostream>
#include <sstream>

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include "PongOutConfig.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow)
{
	static char szAppName[] = "PongOut";
	HWND        hwnd;
	MSG         msg;
	WNDCLASSEX  wndClass;

	wndClass.cbSize			= sizeof(wndClass);
	wndClass.style			= CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc	= WndProc;
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

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

std::string getVersionString()
{
	std::ostringstream stream;
	stream << "PongOut " << PongOut_VERSION_MAJOR << "." << PongOut_VERSION_MINOR << "." << PongOut_VERSION_PATCH;

	return stream.str();
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT	ps;
	HDC				hdc;

	switch (iMsg)
	{
	case WM_PAINT:
		{
			std::string version = getVersionString();
			std::string hello = "Hello World!";
			fs::path programPath(__argv[0]);

			hdc = BeginPaint(hwnd, &ps);
			TextOut(hdc, 100, 100, version.c_str(), version.length());
			TextOut(hdc, 100, 120, hello.c_str(), hello.length());
			TextOut(hdc, 100, 140, programPath.string().c_str(), programPath.string().length());
			EndPaint(hwnd, &ps);
			return 0;
		}

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

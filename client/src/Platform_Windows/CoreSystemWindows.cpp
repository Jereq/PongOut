#include "CoreSystemWindows.h"
static bool	shutDown = false;

CoreSystemWindows::CoreSystemWindows()
	: ICoreSystem(), graphics(NULL)
{

}

CoreSystemWindows::~CoreSystemWindows()
{

}

void CoreSystemWindows::setHwnd(HWND _hWnd)
{
	hWnd = _hWnd;
}

boost::filesystem::path CoreSystemWindows::getRootDir()const
{
	boost::filesystem::path p = "test";
	return p;
}

double CoreSystemWindows::getTime()const
{
	return 42.0;
}

bool CoreSystemWindows::windowIsClosing()const
{
	return shutDown;
}

void CoreSystemWindows::pollEvents()
{
	MSG msg;

	if(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	//WndProc(hWnd, NULL, NULL, NULL);
}

IGraphics* CoreSystemWindows::getGraphics()
{
	if( graphics == NULL )
		graphics = new GraphicsWindows(hWnd);

	return graphics;
}

LRESULT CALLBACK CoreSystemWindows::WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT	ps;
	HDC				hdc;

	switch (iMsg)
	{
	//case WM_PAINT:
	//	{
	//		std::string version = getVersionString();
	//		std::string hello = "Hello World!";
	//		fs::path programPath(__argv[0]);

	//		hdc = BeginPaint(hwnd, &ps);
	//		TextOut(hdc, 100, 100, version.c_str(), version.length());
	//		TextOut(hdc, 100, 120, hello.c_str(), hello.length());
	//		TextOut(hdc, 100, 140, programPath.string().c_str(), programPath.string().length());
	//		EndPaint(hwnd, &ps);
	//		return 0;
	//	}

	case WM_DESTROY:
		PostQuitMessage(0);
		shutDown = true;
		return 0;

		
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

#include "CoreSystemWindows.h"

namespace fs = boost::filesystem;

static bool	shutDown = false;

bool ICoreSystem::init(int _argc, char** _argv)
{
	fs::path fullPath(fs::initial_path<fs::path>());
	fullPath = fs::system_complete(fs::path(_argv[0]));
	
	if (!fs::exists(fullPath))
	{
		std::cout << "Error: Invalid argv[0], stop hacking!" << std::endl;
		return false;
	}

	instance.reset();
	instance.reset(new CoreSystemWindows(fullPath.parent_path()));

	return true;
}

CoreSystemWindows::CoreSystemWindows(const boost::filesystem::path& _rootDir)
	: ICoreSystem(_rootDir), graphics(NULL)
{

}

CoreSystemWindows::~CoreSystemWindows()
{

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

IGraphics::ptr CoreSystemWindows::getGraphics()
{
	if( graphics == NULL )
		graphics.reset(new GraphicsWindows(&WndProc));

	return graphics;
}

LRESULT CALLBACK CoreSystemWindows::WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
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

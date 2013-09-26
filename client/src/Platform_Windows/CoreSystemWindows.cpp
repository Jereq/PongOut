#include "CoreSystemWindows.h"
static bool	shutDown = false;

RAWINPUT* rawInput;
unsigned short	key, oldKey;

CoreSystemWindows::CoreSystemWindows()
	: ICoreSystem(), graphics(NULL)
{
	bool result;

	registerRAW();

	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	if(frequency == 0)
	{
		bool notgood = true;
	}
	ticksPerMs = (float)(frequency / 1000);

	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	soundManager = new SoundManager();
	result = soundManager->initialize();
	if(!result)
	{
		int i = 42;
	}
}

CoreSystemWindows::~CoreSystemWindows()
{

}

SoundManager* CoreSystemWindows::getSounds()
{
	return soundManager;
}

void CoreSystemWindows::setHwnd(HWND _hWnd)
{
	hWnd = _hWnd;
}

bool CoreSystemWindows::isKeyPress(unsigned short _key)
{
	return (key == _key);
}

bool CoreSystemWindows::isNewKeyPress(unsigned short _key)
{
	return ((key == _key) && (key != oldKey));
}

boost::filesystem::path CoreSystemWindows::getRootDir()const
{
	boost::filesystem::path p = "test";
	return p;
}

double CoreSystemWindows::getTime()const
{
	return frameTime;
}

void CoreSystemWindows::registerRAW()
{
	RAWINPUTDEVICE rid[2];

	rid[0].usUsagePage	= 0x01;
	rid[0].usUsage		= 0x02;
	rid[0].dwFlags		= 0;
	rid[0].hwndTarget	= 0;

	rid[1].usUsagePage	= 0x01;
	rid[1].usUsage		= 0x06;
	rid[1].dwFlags		= 0;
	rid[1].hwndTarget	= 0;
	
	DWORD error;
	if(RegisterRawInputDevices(rid, 2, sizeof(rid[0])) == FALSE )
		error = GetLastError();
}

bool CoreSystemWindows::windowIsClosing()const
{
	return shutDown;
}

void CoreSystemWindows::pollEvents()
{
	INT64 currentTime;
	float timeDifference;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	timeDifference = (float)(currentTime - startTime);
	frameTime = timeDifference / ticksPerMs;
	startTime = currentTime;

	MSG msg;
	oldKey = key;

	while(PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);	
	}
}

IGraphics* CoreSystemWindows::getGraphics()
{
	if( graphics == NULL )
		graphics = new GraphicsWindows(hWnd);

	return graphics;
}

LRESULT CALLBACK CoreSystemWindows::WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	
	switch (iMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		shutDown = true;
		return 0;

	case WM_INPUT:
			UINT dwSize;
			GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
			
			LPBYTE lpb = new BYTE[dwSize];
			if(lpb == NULL)
				return 0;

			if(GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
				return 0;

			rawInput = (RAWINPUT*)lpb;

			if(rawInput->header.dwType == RIM_TYPEKEYBOARD)
			{		
				key = rawInput->data.keyboard.VKey;
			}
			else if(rawInput->header.dwType == RIM_TYPEMOUSE)
			{

			}
		break;

	}
	//return CallWindowProc(WndProc, hwnd, iMsg, wParam, lParam);
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

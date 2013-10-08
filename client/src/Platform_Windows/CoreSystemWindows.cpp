#include "CoreSystemWindows.h"
#include <windowsx.h>

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

double CoreSystemWindows::getTime()const
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	return (double)currentTime.QuadPart / frequency;
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

	if (graphics)
	{
		HWND hWnd = graphics->getHWND();
		if (hWnd)
		{
			while(PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE) > 0)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);	
			}
		}
	}
}

IGraphics::ptr CoreSystemWindows::getGraphics()
{
	if( graphics == NULL )
		graphics.reset(new GraphicsWindows(&WndProc));

	return graphics;
}

IInput::ptr CoreSystemWindows::getInput()
{
	if (!input)
	{
		input.reset(new InputWindows());
	}

	return input;
}

LRESULT CALLBACK CoreSystemWindows::WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	std::shared_ptr<InputWindows> input;
	std::shared_ptr<ICoreSystem> instance(getInstance());
	if(instance)
	{
		input = std::static_pointer_cast<InputWindows>(instance->getInput());
	}

	switch (iMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		shutDown = true;
		return 0;

	case WM_INPUT:
		{
			UINT dwSize;
			GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
			
			LPBYTE lpb = new BYTE[dwSize];
			if(lpb == NULL)
				return 0;

			if(GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
				return 0;

			RAWINPUT* rawInput = (RAWINPUT*)lpb;

			if(input)
			{
				if(rawInput->header.dwType == RIM_TYPEKEYBOARD)
				{		
					unsigned short key = rawInput->data.keyboard.VKey;
					bool isDown = rawInput->data.keyboard.Flags == RI_KEY_MAKE;

					input->addFrameKey(key, isDown);
				}
				else if(rawInput->header.dwType == RIM_TYPEMOUSE)
				{
									unsigned short key = rawInput->data.mouse.usButtonFlags;
					if (key & RI_MOUSE_LEFT_BUTTON_DOWN)
					{
						input->addMouseKey(IInput::MouseButtonEvent::Button::LEFT, true);
					}
						
					if (key & RI_MOUSE_LEFT_BUTTON_UP)
					{
						input->addMouseKey(IInput::MouseButtonEvent::Button::LEFT, false);
					}

					if (key & RI_MOUSE_RIGHT_BUTTON_DOWN)
					{
						input->addMouseKey(IInput::MouseButtonEvent::Button::RIGHT, true);
					}

					if (key & RI_MOUSE_RIGHT_BUTTON_UP)
					{
						input->addMouseKey(IInput::MouseButtonEvent::Button::RIGHT, false);
					}

					if (key & RI_MOUSE_MIDDLE_BUTTON_DOWN)
					{
						input->addMouseKey(IInput::MouseButtonEvent::Button::MIDDLE, true);
					}

					if (key & RI_MOUSE_MIDDLE_BUTTON_UP)
					{
						input->addMouseKey(IInput::MouseButtonEvent::Button::MIDDLE, false);
					}
				}

				return 0;
			}
		}
		break;
	case WM_MOUSEMOVE:
		{
			if(input)
			{
				int x = GET_X_LPARAM(lParam);
				int y = GET_Y_LPARAM(lParam);
				input->addMouseMove(x,y);

				return 0;
			}
		}
		break;
	case WM_CHAR:
		{
			if (input)
			{
				char32_t character = wParam;
				input->addCharacter(character);

				return 0;
			}

		}
		break;

	case WM_SIZE:
		if(input)
		{
			unsigned int width = GET_X_LPARAM(lParam);
			unsigned int height = GET_Y_LPARAM(lParam);

			input->onResize(width, height);
			return 0;
		}
		break;
	}

	return DefWindowProcW(hwnd, iMsg, wParam, lParam);
}

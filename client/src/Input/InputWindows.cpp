#include "InputWindows.h"

std::map<unsigned short, IInput::KeyCode> InputWindows::keyMapFromNative;
std::map<IInput::KeyCode, unsigned short> InputWindows::keyMapToNative;

InputWindows::InputWindows()
{
	initKeyMaps();
}

void InputWindows::insertKey(unsigned short _nativeKey, IInput::KeyCode _iKey)
{
	keyMapFromNative[_nativeKey] = _iKey;
	keyMapToNative[_iKey] = _nativeKey;
}

void InputWindows::initKeyMaps()
{
	unsigned short hex = 'A';

	//add alphabet
	for(char i='A'; i<= 'Z'; i++)
	{
		insertKey(i,(KeyCode)i);
	}
	//add numbers 
	//note: windows makes no different between left/right keys for certain virtuals
	//- check scancodes for left/right
	for(char i = '0'; i <= '9'; i++)
	{
		insertKey(i,(KeyCode)i);
	}
	//add virtual keys
	insertKey(VK_OEM_7, IInput::KeyCode::SINGLE_QUOTE);
	insertKey(VK_OEM_102, IInput::KeyCode::BACK_SLASH);
	insertKey(VK_BACK, IInput::KeyCode::BACKSPACE);
	insertKey(VK_CAPITAL, IInput::KeyCode::CAPS_LOCK);
	insertKey(VK_OEM_COMMA, IInput::KeyCode::COMMA);
	insertKey(VK_DELETE, IInput::KeyCode::DEL);
	insertKey(VK_DOWN, IInput::KeyCode::DOWN_ARROW);
	insertKey(VK_END, IInput::KeyCode::END);
	insertKey(VK_RETURN, IInput::KeyCode::RETURN);
	insertKey(VK_OEM_PLUS, IInput::KeyCode::EQUAL);
	insertKey(VK_ESCAPE, IInput::KeyCode::ESCAPE);
	insertKey(VK_F1, IInput::KeyCode::F1);
	insertKey(VK_F2, IInput::KeyCode::F2);
	insertKey(VK_F3, IInput::KeyCode::F3);
	insertKey(VK_F4, IInput::KeyCode::F4);
	insertKey(VK_F5, IInput::KeyCode::F5);
	insertKey(VK_F6, IInput::KeyCode::F6);
	insertKey(VK_F7, IInput::KeyCode::F7);
	insertKey(VK_F8, IInput::KeyCode::F8);
	insertKey(VK_F9, IInput::KeyCode::F9);
	insertKey(VK_F10, IInput::KeyCode::F10);
	insertKey(VK_F11, IInput::KeyCode::F11);
	insertKey(VK_F12, IInput::KeyCode::F12);
	insertKey(VK_OEM_3, IInput::KeyCode::BACK_QUOTE);
	insertKey(VK_HOME, IInput::KeyCode::HOME);
	insertKey(VK_INSERT, IInput::KeyCode::INSERT);
	insertKey(VK_NUMPAD0, IInput::KeyCode::NP0);
	insertKey(VK_NUMPAD1, IInput::KeyCode::NP1);
	insertKey(VK_NUMPAD2, IInput::KeyCode::NP2);
	insertKey(VK_NUMPAD3, IInput::KeyCode::NP3);
	insertKey(VK_NUMPAD4, IInput::KeyCode::NP4);
	insertKey(VK_NUMPAD5, IInput::KeyCode::NP5);
	insertKey(VK_NUMPAD6, IInput::KeyCode::NP6);
	insertKey(VK_NUMPAD7, IInput::KeyCode::NP7);
	insertKey(VK_NUMPAD8, IInput::KeyCode::NP8);
	insertKey(VK_NUMPAD9, IInput::KeyCode::NP9);
	insertKey(VK_ADD, IInput::KeyCode::NP_PLUS);
	insertKey(VK_DECIMAL, IInput::KeyCode::NP_PERIOD);
	insertKey(VK_DIVIDE, IInput::KeyCode::NP_DIVIDE); 
	//insertKey(VK_KP_ENTER, IInput::KeyCode::NP_RETURN);
	insertKey(VK_MULTIPLY, IInput::KeyCode::NP_MULTIPLY);
	insertKey(VK_SUBTRACT, IInput::KeyCode::NP_MINUS);
	insertKey(VK_LEFT, IInput::KeyCode::LEFT_ARROW);
	insertKey(VK_MENU, IInput::KeyCode::LEFT_ALT);
	insertKey(VK_OEM_4, IInput::KeyCode::LEFT_SQUARE_BRACKET);
	insertKey(VK_CONTROL, IInput::KeyCode::LEFT_CTRL);
	insertKey(VK_SHIFT, IInput::KeyCode::LEFT_SHIFT);
	insertKey(VK_LWIN, IInput::KeyCode::LEFT_SUPER);
	insertKey(VK_MENU, IInput::KeyCode::MENU);
	insertKey(VK_OEM_MINUS, IInput::KeyCode::DASH);
	insertKey(VK_NUMLOCK, IInput::KeyCode::NUM_LOCK);
	insertKey(VK_NEXT, IInput::KeyCode::PAGE_DOWN);
	insertKey(VK_PRIOR, IInput::KeyCode::PAGE_UP);
	insertKey(VK_PAUSE, IInput::KeyCode::BREAK);
	insertKey(VK_OEM_PERIOD, IInput::KeyCode::PERIOD);
	insertKey(VK_PRINT, IInput::KeyCode::SYS_RQ);
	insertKey(VK_RIGHT, IInput::KeyCode::RIGHT_ARROW);
	//insertKey(VK_RIGHT_ALT, IInput::KeyCode::RIGHT_ALT);
	insertKey(VK_OEM_6, IInput::KeyCode::RIGHT_SQUARE_BRACKET);
	//insertKey(VK_RCONTROL, IInput::KeyCode::RIGHT_CTRL);
	//insertKey(VK_RSHIFT, IInput::KeyCode::RIGHT_SHIFT);
	insertKey(VK_RWIN, IInput::KeyCode::RIGHT_SUPER);
	insertKey(VK_SCROLL, IInput::KeyCode::SCROLL_LOCK);
	insertKey(VK_OEM_1, IInput::KeyCode::SEMICOLON);
	insertKey(VK_OEM_2, IInput::KeyCode::SLASH);
	insertKey(VK_SPACE, IInput::KeyCode::SPACE);
	insertKey(VK_TAB, IInput::KeyCode::TAB);
	insertKey(VK_UP, IInput::KeyCode::UP_ARROW);
	insertKey(VK_OEM_5, IInput::KeyCode::WORLD_1);
	insertKey(VK_OEM_8, IInput::KeyCode::WORLD_2);
}

IInput::KeyCode InputWindows::translateKey(unsigned short _nativeKey)
{
	if(keyMapFromNative.count(_nativeKey) == 0)
	{
		return IInput::KeyCode::UNKNOWN;
	}

	return keyMapFromNative.at(_nativeKey);
}

void InputWindows::addFrameKey(unsigned short _key, bool _isDown)
{
	Event e;
	e.type = Event::Type::KEY;
	e.keyEvent.key = translateKey(_key);
	e.keyEvent.pressed = _isDown;
	
	events.push_back(e);
}

void InputWindows::addMouseKey(IInput::MouseButtonEvent::Button _key, bool _isDown)
{
	Event e;
	e.type = Event::Type::MOUSE_BUTTON;
	e.mouseButtonEvent.button = _key;
	e.mouseButtonEvent.pressed = _isDown;
	events.push_back(e);
}

void InputWindows::addMouseMove(int _lastX, int _lastY)
{
	Event e;
	e.type = Event::Type::MOUSE_MOVE;
	e.mouseMoveEvent.posX = (double)_lastX;
	e.mouseMoveEvent.posY = (double)_lastY;
	events.push_back(e);
}

void InputWindows::addCharacter(char32_t _char)
{
	Event e;
	e.type = Event::Type::CHARACTER;
	e.charEvent.character = _char;
	events.push_back(e);
}
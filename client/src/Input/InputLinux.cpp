#include "InputLinux.h"

#include <GLFW/glfw3.h>

std::map<int, IInput::KeyCode> InputLinux::keyMapFromNative;
std::map<IInput::KeyCode, int> InputLinux::keyMapToNative;
std::map<GLFWwindow*, std::weak_ptr<InputLinux>> InputLinux::windowMap;
bool InputLinux::staticInitialized = initKeyMaps();

void InputLinux::insertKey(int _nativeKey, IInput::KeyCode _iKey)
{
	keyMapFromNative[_nativeKey] = _iKey;
	keyMapToNative[_iKey] = _nativeKey;
}

bool InputLinux::initKeyMaps()
{
	insertKey(GLFW_KEY_0, IInput::KeyCode::K0);
	insertKey(GLFW_KEY_1, IInput::KeyCode::K1);
	insertKey(GLFW_KEY_2, IInput::KeyCode::K2);
	insertKey(GLFW_KEY_3, IInput::KeyCode::K3);
	insertKey(GLFW_KEY_4, IInput::KeyCode::K4);
	insertKey(GLFW_KEY_5, IInput::KeyCode::K5);
	insertKey(GLFW_KEY_6, IInput::KeyCode::K6);
	insertKey(GLFW_KEY_7, IInput::KeyCode::K7);
	insertKey(GLFW_KEY_8, IInput::KeyCode::K8);
	insertKey(GLFW_KEY_9, IInput::KeyCode::K9);
	insertKey(GLFW_KEY_A, IInput::KeyCode::A);
	insertKey(GLFW_KEY_B, IInput::KeyCode::B);
	insertKey(GLFW_KEY_C, IInput::KeyCode::C);
	insertKey(GLFW_KEY_D, IInput::KeyCode::D);
	insertKey(GLFW_KEY_E, IInput::KeyCode::E);
	insertKey(GLFW_KEY_F, IInput::KeyCode::F);
	insertKey(GLFW_KEY_G, IInput::KeyCode::G);
	insertKey(GLFW_KEY_H, IInput::KeyCode::H);
	insertKey(GLFW_KEY_I, IInput::KeyCode::I);
	insertKey(GLFW_KEY_J, IInput::KeyCode::J);
	insertKey(GLFW_KEY_K, IInput::KeyCode::K);
	insertKey(GLFW_KEY_L, IInput::KeyCode::L);
	insertKey(GLFW_KEY_M, IInput::KeyCode::M);
	insertKey(GLFW_KEY_N, IInput::KeyCode::N);
	insertKey(GLFW_KEY_O, IInput::KeyCode::O);
	insertKey(GLFW_KEY_P, IInput::KeyCode::P);
	insertKey(GLFW_KEY_Q, IInput::KeyCode::Q);
	insertKey(GLFW_KEY_R, IInput::KeyCode::R);
	insertKey(GLFW_KEY_S, IInput::KeyCode::S);
	insertKey(GLFW_KEY_T, IInput::KeyCode::T);
	insertKey(GLFW_KEY_U, IInput::KeyCode::U);
	insertKey(GLFW_KEY_V, IInput::KeyCode::V);
	insertKey(GLFW_KEY_W, IInput::KeyCode::W);
	insertKey(GLFW_KEY_X, IInput::KeyCode::X);
	insertKey(GLFW_KEY_Y, IInput::KeyCode::Y);
	insertKey(GLFW_KEY_Z, IInput::KeyCode::Z);
	insertKey(GLFW_KEY_APOSTROPHE, IInput::KeyCode::SINGLE_QUOTE);
	insertKey(GLFW_KEY_BACKSLASH, IInput::KeyCode::BACK_SLASH);
	insertKey(GLFW_KEY_BACKSPACE, IInput::KeyCode::BACKSPACE);
	insertKey(GLFW_KEY_CAPS_LOCK, IInput::KeyCode::CAPS_LOCK);
	insertKey(GLFW_KEY_COMMA, IInput::KeyCode::COMMA);
	insertKey(GLFW_KEY_DELETE, IInput::KeyCode::DEL);
	insertKey(GLFW_KEY_DOWN, IInput::KeyCode::DOWN_ARROW);
	insertKey(GLFW_KEY_END, IInput::KeyCode::END);
	insertKey(GLFW_KEY_ENTER, IInput::KeyCode::RETURN);
	insertKey(GLFW_KEY_EQUAL, IInput::KeyCode::EQUAL);
	insertKey(GLFW_KEY_ESCAPE, IInput::KeyCode::ESCAPE);
	insertKey(GLFW_KEY_F1, IInput::KeyCode::F1);
	insertKey(GLFW_KEY_F2, IInput::KeyCode::F2);
	insertKey(GLFW_KEY_F3, IInput::KeyCode::F3);
	insertKey(GLFW_KEY_F4, IInput::KeyCode::F4);
	insertKey(GLFW_KEY_F5, IInput::KeyCode::F5);
	insertKey(GLFW_KEY_F6, IInput::KeyCode::F6);
	insertKey(GLFW_KEY_F7, IInput::KeyCode::F7);
	insertKey(GLFW_KEY_F8, IInput::KeyCode::F8);
	insertKey(GLFW_KEY_F9, IInput::KeyCode::F9);
	insertKey(GLFW_KEY_F10, IInput::KeyCode::F10);
	insertKey(GLFW_KEY_F11, IInput::KeyCode::F11);
	insertKey(GLFW_KEY_F12, IInput::KeyCode::F12);
	insertKey(GLFW_KEY_GRAVE_ACCENT, IInput::KeyCode::BACK_QUOTE);
	insertKey(GLFW_KEY_HOME, IInput::KeyCode::HOME);
	insertKey(GLFW_KEY_INSERT, IInput::KeyCode::INSERT);
	insertKey(GLFW_KEY_KP_0, IInput::KeyCode::NP0);
	insertKey(GLFW_KEY_KP_1, IInput::KeyCode::NP1);
	insertKey(GLFW_KEY_KP_2, IInput::KeyCode::NP2);
	insertKey(GLFW_KEY_KP_3, IInput::KeyCode::NP3);
	insertKey(GLFW_KEY_KP_4, IInput::KeyCode::NP4);
	insertKey(GLFW_KEY_KP_5, IInput::KeyCode::NP5);
	insertKey(GLFW_KEY_KP_6, IInput::KeyCode::NP6);
	insertKey(GLFW_KEY_KP_7, IInput::KeyCode::NP7);
	insertKey(GLFW_KEY_KP_8, IInput::KeyCode::NP8);
	insertKey(GLFW_KEY_KP_9, IInput::KeyCode::NP9);
	insertKey(GLFW_KEY_KP_ADD, IInput::KeyCode::NP_PLUS);
	insertKey(GLFW_KEY_KP_DECIMAL, IInput::KeyCode::NP_PERIOD);
	insertKey(GLFW_KEY_KP_DIVIDE, IInput::KeyCode::NP_DIVIDE);
	insertKey(GLFW_KEY_KP_ENTER, IInput::KeyCode::NP_RETURN);
	insertKey(GLFW_KEY_KP_MULTIPLY, IInput::KeyCode::NP_MULTIPLY);
	insertKey(GLFW_KEY_KP_SUBTRACT, IInput::KeyCode::NP_MINUS);
	insertKey(GLFW_KEY_LEFT, IInput::KeyCode::LEFT_ARROW);
	insertKey(GLFW_KEY_LEFT_ALT, IInput::KeyCode::LEFT_ALT);
	insertKey(GLFW_KEY_LEFT_BRACKET, IInput::KeyCode::LEFT_SQUARE_BRACKET);
	insertKey(GLFW_KEY_LEFT_CONTROL, IInput::KeyCode::LEFT_CTRL);
	insertKey(GLFW_KEY_LEFT_SHIFT, IInput::KeyCode::LEFT_SHIFT);
	insertKey(GLFW_KEY_LEFT_SUPER, IInput::KeyCode::LEFT_SUPER);
	insertKey(GLFW_KEY_MENU, IInput::KeyCode::MENU);
	insertKey(GLFW_KEY_MINUS, IInput::KeyCode::DASH);
	insertKey(GLFW_KEY_NUM_LOCK, IInput::KeyCode::NUM_LOCK);
	insertKey(GLFW_KEY_PAGE_DOWN, IInput::KeyCode::PAGE_DOWN);
	insertKey(GLFW_KEY_PAGE_UP, IInput::KeyCode::PAGE_UP);
	insertKey(GLFW_KEY_PAUSE, IInput::KeyCode::BREAK);
	insertKey(GLFW_KEY_PERIOD, IInput::KeyCode::PERIOD);
	insertKey(GLFW_KEY_PRINT_SCREEN, IInput::KeyCode::SYS_RQ);
	insertKey(GLFW_KEY_RIGHT, IInput::KeyCode::RIGHT_ARROW);
	insertKey(GLFW_KEY_RIGHT_ALT, IInput::KeyCode::RIGHT_ALT);
	insertKey(GLFW_KEY_RIGHT_BRACKET, IInput::KeyCode::RIGHT_SQUARE_BRACKET);
	insertKey(GLFW_KEY_RIGHT_CONTROL, IInput::KeyCode::RIGHT_CTRL);
	insertKey(GLFW_KEY_RIGHT_SHIFT, IInput::KeyCode::RIGHT_SHIFT);
	insertKey(GLFW_KEY_RIGHT_SUPER, IInput::KeyCode::RIGHT_SUPER);
	insertKey(GLFW_KEY_SCROLL_LOCK, IInput::KeyCode::SCROLL_LOCK);
	insertKey(GLFW_KEY_SEMICOLON, IInput::KeyCode::SEMICOLON);
	insertKey(GLFW_KEY_SLASH, IInput::KeyCode::SLASH);
	insertKey(GLFW_KEY_SPACE, IInput::KeyCode::SPACE);
	insertKey(GLFW_KEY_TAB, IInput::KeyCode::TAB);
	insertKey(GLFW_KEY_UP, IInput::KeyCode::UP_ARROW);
	insertKey(GLFW_KEY_WORLD_1, IInput::KeyCode::WORLD_1);
	insertKey(GLFW_KEY_WORLD_2, IInput::KeyCode::WORLD_2);

	return true;
}

IInput::KeyCode InputLinux::translateKey(int _nativeKey)
{
	if (keyMapFromNative.count(_nativeKey) == 0)
	{
		return IInput::KeyCode::UNKNOWN;
	}

	return keyMapFromNative.at(_nativeKey);
}

int InputLinux::translateKey(IInput::KeyCode _iKey)
{
	if (keyMapToNative.count(_iKey) == 0)
	{
		return GLFW_KEY_UNKNOWN;
	}

	return keyMapToNative.at(_iKey);
}

InputLinux::InputLinux()
{
}

void InputLinux::keyCallback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods)
{
	if (_action == GLFW_REPEAT)
	{
		return;
	}

	Event ev;
	ev.type = Event::Type::KEY;
	ev.keyEvent.key = translateKey(_key);
	ev.keyEvent.pressed = (_action == GLFW_PRESS);

	ptr input = windowMap[_window].lock();
	if (input)
	{
		input->events.push_back(ev);
	}
}

void InputLinux::mouseButtonCallback(GLFWwindow* _window, int _button, int _action, int _mods)
{
	MouseButtonEvent::Button button;
	switch (_button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:	button = MouseButtonEvent::Button::LEFT;	break;
	case GLFW_MOUSE_BUTTON_RIGHT:	button = MouseButtonEvent::Button::RIGHT;	break;
	case GLFW_MOUSE_BUTTON_MIDDLE:	button = MouseButtonEvent::Button::MIDDLE;	break;
	default:						button = MouseButtonEvent::Button::UNKNOWN; break;
	}

	Event ev;
	ev.type = Event::Type::MOUSE_BUTTON;
	ev.mouseButtonEvent.button = button;
	ev.mouseButtonEvent.pressed = (_action == GLFW_PRESS);

	ptr input = windowMap[_window].lock();
	if (input)
	{
		input->events.push_back(ev);
	}
}

void InputLinux::mouseCursorPosCallback(GLFWwindow* _window, double _xpos, double _ypos)
{
	Event ev;
	ev.type = Event::Type::MOUSE_MOVE;
	ev.mouseMoveEvent.posX = _xpos;
	ev.mouseMoveEvent.posY = _ypos;

	ptr input = windowMap[_window].lock();
	if (input)
	{
		input->events.push_back(ev);
	}
}

void InputLinux::characterCallback(GLFWwindow* _window, unsigned int _character)
{
	Event ev;
	ev.type = Event::Type::CHARACTER;
	ev.charEvent.character = _character;

	ptr input = windowMap[_window].lock();
	if (input)
	{
		input->events.push_back(ev);
	}
}

bool InputLinux::registerWindowForInput(GLFWwindow* _window)
{
	if (!_window)
	{
		return false;
	}

	windowMap[_window] = shared_from_this();

	glfwSetKeyCallback(_window, &keyCallback);
	glfwSetCursorPosCallback(_window, &mouseCursorPosCallback);
	glfwSetMouseButtonCallback(_window, &mouseButtonCallback);
	glfwSetCharCallback(_window, &characterCallback);

	return true;
}


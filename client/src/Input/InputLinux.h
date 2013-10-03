#pragma once

#include "IInput.h"

#include <GLFW/glfw3.h>
#include <map>

class InputLinux : public IInput, public std::enable_shared_from_this<InputLinux>
{
private:
	static std::map<int, IInput::KeyCode> keyMapFromNative;
	static std::map<IInput::KeyCode, int> keyMapToNative;
	static std::map<GLFWwindow*, std::weak_ptr<InputLinux>> windowMap;
	static bool staticInitialized;

	static void insertKey(int _nativeKey, IInput::KeyCode _iKey);
	static bool initKeyMaps();
	static IInput::KeyCode translateKey(int _nativeKey);
	static int translateKey(IInput::KeyCode _iKey);

public:
	typedef std::shared_ptr<InputLinux> ptr;

	InputLinux();

	static void keyCallback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods);
	static void mouseButtonCallback(GLFWwindow* _window, int _button, int _action, int _mods);
	static void mouseCursorPosCallback(GLFWwindow* _window, double _xpos, double _ypos);
	static void characterCallback(GLFWwindow* _window, unsigned int _character);

	bool registerWindowForInput(GLFWwindow* _window);
};

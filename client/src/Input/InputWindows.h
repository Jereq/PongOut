#pragma once

#include "IInput.h"

#include <map>
#include <Windows.h>
#include <vector>

class InputWindows : public IInput
{
private:
	static std::map<unsigned short, IInput::KeyCode> keyMapFromNative;
	static std::map<IInput::KeyCode, unsigned short> keyMapToNative;

	static void insertKey(unsigned short _nativeKey, IInput::KeyCode _iKey);
	static void initKeyMaps();
	static IInput::KeyCode translateKey(unsigned short _nativeKey);
	static unsigned short translateKey(IInput::KeyCode _iKey);
	unsigned int screenWidth, screenHeight;

public:
	InputWindows();
	typedef std::shared_ptr<InputWindows> ptr;
	void addFrameKey(unsigned short _key, bool _isDown);
	void addMouseKey(IInput::MouseButtonEvent::Button _key, bool _isDown);
	void addMouseMove(int _lastX, int _lastY);
	void addCharacter(char32_t _char);
	void onResize(unsigned int _width, unsigned int _height);
};

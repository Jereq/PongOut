#pragma once

#include "IInput.h"

#include <map>

class InputLinux : public IInput
{
private:
	static std::map<short, IInput::KeyCode> keyMapFromNative;
	static std::map<IInput::KeyCode, short> keyMapToNative;

	static void insertKey(short _nativeKey, IInput::KeyCode _iKey);
	static void initKeyMaps();
	static IInput::KeyCode translateKey(short _nativeKey);
	static short translateKey(IInput::KeyCode _iKey);

public:
	typedef std::shared_ptr<InputLinux> ptr;
};

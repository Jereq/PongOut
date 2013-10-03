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

public:
	typedef std::shared_ptr<InputWindows> ptr;
};

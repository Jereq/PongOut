#include "InputWindows.h"

std::map<unsigned short, IInput::KeyCode> InputWindows::keyMapFromNative;
std::map<IInput::KeyCode, unsigned short> InputWindows::keyMapToNative;

void InputWindows::insertKey(unsigned short _nativeKey, IInput::KeyCode _iKey)
{
	keyMapFromNative[_nativeKey] = _iKey;
	keyMapToNative[_iKey] = _nativeKey;
}

void InputWindows::initKeyMaps()
{
	unsigned short hex = 'A';

	//alphabet
	for(char i='A'; i<= 'Z'; i++)
	{
		insertKey(i,(KeyCode)i);
	}
}
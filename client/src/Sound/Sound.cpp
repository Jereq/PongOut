#include "Sound.h"
#include <fmod.hpp>

Sound::Sound(string _resourceName /* = "undefined" */, string _type, float _volume /* = 1.0f */)
{
	resourceName = _resourceName;
	type = _type;
	volume = _volume;
}

Sound::~Sound(void)
{
}
#include "Sound.h"
#include <fmod.hpp>

Sound::Sound(Sound& _other)
	: volume(_other.volume),
	resourceName(_other.resourceName),
	type(_other.type),
	sound(_other.sound)
{
_other.sound = nullptr;
}

Sound::Sound(Sound &&_other)
	: volume(_other.volume),
	resourceName(_other.resourceName),
	type(_other.type),
	sound(_other.sound)
{
	_other.sound = nullptr;
}

Sound::Sound(string _resourceName /* = "undefined" */, string _type, float _volume /* = 1.0f */)
	: sound(nullptr),
	resourceName(_resourceName),
	type(_type),
	volume(_volume)
{
}

Sound::~Sound(void)
{
	if(sound)
	{
		sound->release();
		sound = nullptr;
	}
}

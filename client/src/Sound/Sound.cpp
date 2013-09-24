#include "Sound.h"

Sound::Sound(string _filename /* = "undefined" */, string _type, float _volume /* = 1.0f */)
{
	filename = _filename;
	type = _type;
	volume = _volume;
}

Sound::~Sound(void)
{
}

float Sound::getVolume(void) const
{
	return volume;
}

void Sound::setVolume(float _volume)
{
	volume = _volume;
}
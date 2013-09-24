#include "SoundSettings.h"

SoundSettings::SoundSettings(float _musicVolume /* = 1.0f */, float _sfxVolume /* = 1.0f */)
{
	musicVolume = _musicVolume;
	sfxVolume = _sfxVolume;
}

SoundSettings::~SoundSettings(void)
{
}

float SoundSettings::getMusicVolume(void) const
{
	return musicVolume;
}

void SoundSettings::setMusicVolume(float _volume)
{
	musicVolume = _volume;
}

float SoundSettings::getSfxVolume(void) const
{
	return sfxVolume;
}

void SoundSettings::setSfxVolume(float _volume)
{
	sfxVolume = _volume;
}
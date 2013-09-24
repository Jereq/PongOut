#pragma once

class SoundSettings
{
private:
	float musicVolume;
	float sfxVolume;
	
public:
	SoundSettings(float _musicVolume = 0.8f, float _sfxVolume = 0.8f);
	~SoundSettings(void);
	
	float getMusicVolume(void) const;
	void setMusicVolume(float _volume);
	float getSfxVolume(void) const;
	void setSfxVolume(float _volume);
};
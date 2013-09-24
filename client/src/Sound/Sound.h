#pragma once
#include <string>
#include <fmod.hpp>

using std::string;

class Sound
{
private:
	string filename;
	string type;
	float volume;
	FMOD::Sound *sound;
	FMOD::Channel *channel;

public:
	Sound(string _filename = "undefined", string _type = "undefined", float _volume = 1.0f);
	~Sound(void);
	
	float getVolumevoid(void) const;
	void setVolume(float _volume);
};
#pragma once
#include <string>

using std::string;

namespace FMOD
{
	class Sound;
	class Channel;
}

class Sound
{
private:
	//string resourceName;
	string type;
	float volume;
	//FMOD::Sound *sound;
	//FMOD::Channel *channel;

public:
	Sound(string _filename = "undefined", string _type = "undefined", float _volume = 1.0f);
	~Sound(void);
	FMOD::Channel *channel;
	FMOD::Sound *sound;
	string resourceName;
	float getVolume(void) const;
	void setVolume(float _volume);
	//FMOD::Channel* getChannel();
};
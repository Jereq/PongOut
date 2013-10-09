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
public:
	Sound(string _resourceName = "undefined", string _type = "undefined",
		float _volume = 1.0f);
	~Sound(void);

	float volume;
	string resourceName;
	string type;
	FMOD::Channel *channel;
	FMOD::Sound *sound;
};
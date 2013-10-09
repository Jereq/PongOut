#pragma once
#include <string>

using std::string;

namespace FMOD
{
	class Sound;
}

class Sound
{
public:
	Sound(Sound &&_other);
	Sound(string _resourceName = "undefined", string _type = "undefined",
		float _volume = 1.0f);
	~Sound(void);


	float volume;
	string resourceName;
	string type;
	FMOD::Sound *sound;

	Sound(const Sound&) = delete;
	Sound& operator=(const Sound&) = delete;
};

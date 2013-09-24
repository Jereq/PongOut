#pragma once
#include <vector>
#include <string>
#include <fmod.hpp>
#include "SoundSettings.h"
#include "Sound.h"

#define MAX_CHANNELS 32

using std::vector;
using std::string;

class SoundManager
{
private:
	FMOD::System *system;
	vector<Sound*> sounds;
	SoundSettings *settings;
public:
	SoundManager(void);
	~SoundManager(void);

	bool initialize(void);
	void shutdown(void);

	void errorReport(string filename, string errorMessage);

};
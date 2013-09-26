#pragma once
#include <vector>
#include <string>
#include "SoundSettings.h"
#include "Sound.h"
#include <fstream>

#define MAX_CHANNELS 32

using std::vector;
using std::string;
using std::ofstream;

namespace FMOD 
{
	class System;
}

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

	bool initializeSound(Sound* _sound);
	bool load(string _filename);
	void play(string _filename);


	void errorReport(string filename, string errorMessage);

};
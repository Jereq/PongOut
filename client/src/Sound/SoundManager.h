#pragma once
#include <vector>
#include <string>
#include "SoundSettings.h"
#include "Sound.h"
#include <fstream>
#include <ResourceLoader/ResourceLoader.h>
#include <boost/filesystem/path.hpp>

#define MAX_CHANNELS 32

using std::vector;
using std::string;
using std::ofstream;

namespace FMOD 
{
	class System;
	class Channel;
}

class SoundManager
{
private:
	FMOD::System *system;
	FMOD::Channel *backgroundChannel;
	vector<Sound> music;
	vector<Sound> sfx;
	SoundSettings settings;
	bool soundsLoaded;
public:
	SoundManager(void);
	~SoundManager(void);

	SoundManager(const SoundManager&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;

	bool initialize(void);
	void shutdown(void);

	bool loadSounds(const boost::filesystem::path &_resourceDir);
	void playSfx(const string _resourceName);
	void changeBackgroundMusic(const string _resourceName);
	
private:
	bool load(const ResourceLoader::Resource &_soundRes);
	void errorReport(const string filename, const string errorMessage);
};

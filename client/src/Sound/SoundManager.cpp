#include "SoundManager.h"
#include <ResourceLoader/ResourceLoader.h>
#include <fmod.hpp>

SoundManager::SoundManager(void)
	: settings(1.f, 1.f),
	system(nullptr),
	backgroundChannel(nullptr),
	soundsLoaded(false)
{
}

SoundManager::~SoundManager(void)
{
	if (system)
	{
		shutdown();
	}
}

bool SoundManager::initialize(void)
{
	if (system)
	{
		return false;
	}

	FMOD_RESULT result;
	unsigned int usedVersion;

	settings = SoundSettings(1.f, 1.f);

	result = FMOD::System_Create(&system);
	if(result != FMOD_OK)
	{
		string errorMessage = "Failed to create FMOD System.";
		errorReport("sound_error.txt", errorMessage);
		return false;
	}

	result = system->getVersion(&usedVersion);
	if(usedVersion < FMOD_VERSION)
	{
		system->release();
		system = nullptr;

		string errorMessage = "Your version of FMOD is not supported: v."+ 
			std::to_string(usedVersion) + "Please update to: " + std::to_string(FMOD_VERSION);
		errorReport("sound_error.txt", errorMessage);
		return false;
	}

	result = system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
	if(result != FMOD_OK)
	{
		string errorMessage = "Failed to set FMOD Speaker mode.";
		errorReport("sound_error.txt", errorMessage);
		return false;
	}
	result = system->init(MAX_CHANNELS, FMOD_INIT_NORMAL, 0);
	if(result != FMOD_OK)
	{
		string errorMessage = "Failed to initialize FMOD System.";
		errorReport("sound_error.txt", errorMessage);
		return false;
	}
	
	system->getChannel(0, &backgroundChannel);

	return true;
}

void SoundManager::shutdown(void)
{
	if(system)
	{
		sfx.clear();
		music.clear();
		soundsLoaded = false;
		system->release();
		system = nullptr;
	}
}

bool SoundManager::loadSounds(const boost::filesystem::path &_resourceDir)
{
	if(!soundsLoaded)
	{
		std::vector<ResourceLoader::Resource> soundResources;
		ResourceLoader::ErrorCode err = ResourceLoader::getResources(soundResources, _resourceDir);

		if(err != ResourceLoader::ErrorCode::SUCCESS)
		{
			string errorMessage = "Failed to load sound resources (" + std::to_string((int)err) + ")";
			errorReport("sound_error.txt", errorMessage);
			return false;
		}

		for(const ResourceLoader::Resource &res : soundResources)
		{
			if (res.type == "music")
			{
				load(res);
			}
			else if (res.type == "sfx")
			{
				load(res);
			}
		}
		soundsLoaded = true;
		
		return true;
	}
	
	return false;
}

void SoundManager::playSfx(const string _resourceName)
{
	int index = -1;

	for(unsigned int i = 0; i < sfx.size(); i++ )
	{
		if(sfx[i].resourceName == _resourceName )
		{
			index = i;
			break;
		}
	}

	FMOD_RESULT res;
	if( index != -1 )
	{
		Sound &sound = sfx[index];
		if(sound.sound == nullptr)
		{
			return;
		}

		if(sound.type != "sfx")
		{
			return;
		}

		FMOD::Channel *channel;
		res = system->playSound(FMOD_CHANNELINDEX::FMOD_CHANNEL_FREE, sfx[index].sound, true, &channel);
		if (res != FMOD_OK)
		{
			//TODO: Add error report.
		}
		channel->setVolume(sound.volume * settings.getSfxVolume());
		channel->setPaused(false);
	}
}

void SoundManager::changeBackgroundMusic(const string _resourceName)
{
	int index = -1;

	for(unsigned int i = 0; i < music.size(); i++)
	{
		if(music[i].resourceName == _resourceName)
		{
			index = i;
			break;
		}
	}

	FMOD_RESULT res;
	if( index != -1 )
	{
		Sound &sound = music[index];
		if(sound.sound == nullptr)
		{
			return;
		}

		if(sound.type != "music")
		{
			return;
		}

		FMOD::Sound *currentSound; 
		res = backgroundChannel->getCurrentSound(&currentSound);
		if(res != FMOD_OK)
		{
			//TODO: Add error report.
		}
		if(sound.sound == currentSound)
		{
			return;
		}
		res = system->playSound(FMOD_CHANNELINDEX::FMOD_CHANNEL_REUSE, music[index].sound, true, &backgroundChannel);
		backgroundChannel->setVolume(sound.volume * settings.getMusicVolume());
		backgroundChannel->setPaused(false);
	}
}

bool SoundManager::load(const ResourceLoader::Resource &_soundRes)
{
	if(_soundRes.type == "music")
	{
		music.push_back(Sound(_soundRes.name, _soundRes.type, 1.0f));
		FMOD_RESULT res = system->createSound(_soundRes.path.string().c_str(), FMOD_LOOP_NORMAL, nullptr, &music.back().sound);
		if(res != FMOD_OK)
		{
			return false;
		}

		return true;
	}
	else if(_soundRes.type == "sfx")
	{
		sfx.push_back(Sound(_soundRes.name, _soundRes.type, 1.0f));
		FMOD_RESULT res = system->createSound(_soundRes.path.string().c_str(), FMOD_DEFAULT, nullptr, &sfx.back().sound);
		if(res != FMOD_OK)
		{
			return false;
		}

		return true;
	}

	return false;
}

void SoundManager::errorReport(const string filename, const string errorMessage)
{
	ofstream errorFile;

	errorFile.open(filename);
	errorFile << errorMessage;
	errorFile.close();

	//TODO: Maybe add a messagebox as well?
}
#include "SoundManager.h"

SoundManager::SoundManager(void)
{
	system = NULL;
	settings = NULL;
}

SoundManager::~SoundManager(void)
{
}

bool SoundManager::initialize(void)
{
	FMOD_RESULT result;
	unsigned int usedVersion;

	settings = new SoundSettings();
	if(!settings)
		return false;

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
		string errorMessage = "Your version of FMOD is not supported: v."+
			std::to_string(usedVersion) + "Please update to: " + std::to_string(FMOD_VERSION);
		errorReport("sound_error.txt", errorMessage);
		return false;
	}

	result = system->setOutput(FMOD_OUTPUTTYPE_WASAPI);
	result = system->init(MAX_CHANNELS, FMOD_INIT_NORMAL, 0);
	result = system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);

	return true;
}

void SoundManager::errorReport(string filename, string errorMessage)
{
	ofstream errorFile;

	errorFile.open(filename);
	errorFile << errorMessage;
	errorFile.close();

	//TODO: Maybe add a messagebox as well?
}
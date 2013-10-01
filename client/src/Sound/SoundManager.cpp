#include "SoundManager.h"

#include <fmod.hpp>

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
	result = system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
	result = system->init(MAX_CHANNELS, FMOD_INIT_NORMAL, 0);
	
	
	return true;
}

bool SoundManager::load(string _filename)
{
	string path = "../resources/sounds/" + _filename + ".mid";

	sounds.push_back( new Sound( _filename, "music", 1.0f ) );
	int current = sounds.size() - 1;

	FMOD_RESULT res = system->createStream( path.c_str(), 0, NULL, &sounds[current]->sound );
	sounds[current]->sound->setMode(FMOD_LOOP_NORMAL);

	
	//return res;
	//initializeSound(sounds[current]);
	return true;
}

bool SoundManager::initializeSound(Sound* _sound)
{
	if(_sound == NULL)
		return false;

	//int c = _sound->id;

	FMOD_RESULT res;

	bool b;
	_sound->channel->isPlaying(&b);
	if( b == false )
		res = system->playSound( FMOD_CHANNELINDEX::FMOD_CHANNEL_REUSE, NULL, false, &_sound->channel );


	//FMOD_DSP_DESCRIPTION dspdesc;
	////FMOD::DSP*	mydsp;

	//memset(&dspdesc, 0, sizeof(FMOD_DSP_DESCRIPTION));
	//strcpy(dspdesc.name, "Music");
	//dspdesc.channels =	0;
	//dspdesc.read	 = DSPDefault;
	//dspdesc.userdata = (Sound*)p_sounds[c];

	//res = p_system->createDSP(&dspdesc, &p_sounds[c]->mydsp);
	//p_sounds[c]->mydsp->setBypass(false);
	//res = p_sounds[c]->channel->addDSP(p_sounds[c]->mydsp,0);


	return true;
}

void SoundManager::play(string _filename)
{
	int index = -1;

	for( int i = 0; i < sounds.size(); i++ )
	{
		if( sounds[i]->resourceName == _filename )
		{
			index = i;
			break;
		}
	}

	FMOD_RESULT res;
	if( index != -1 )
	{
		bool b;
		sounds[index]->channel->isPlaying(&b);
		if( b == false )
		{	
			res = system->playSound( FMOD_CHANNELINDEX::FMOD_CHANNEL_REUSE, sounds[index]->sound, false, &sounds[index]->channel );
			//sounds[index]->mydsp->setBypass(false);
			//sounds[index]->channel->addDSP(p_sounds[index]->mydsp,0);
		}
	}

}

void SoundManager::errorReport(string filename, string errorMessage)
{
	ofstream errorFile;

	errorFile.open(filename);
	errorFile << errorMessage;
	errorFile.close();

	//TODO: Maybe add a messagebox as well?
}
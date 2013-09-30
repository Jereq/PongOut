#include "ICoreSystem.h"

ICoreSystem::ICoreSystem()
	: soundManager(nullptr)
{
}

SoundManager* ICoreSystem::getSounds()
{
	if (!soundManager)
	{
		soundManager = new SoundManager();
	}

	return soundManager;
}

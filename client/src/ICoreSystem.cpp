#include "ICoreSystem.h"

ICoreSystem::ICoreSystem()
	: soundManager(nullptr)
{
}

SoundManager* ICoreSystem::getSounds()
{
	return soundManager;
}
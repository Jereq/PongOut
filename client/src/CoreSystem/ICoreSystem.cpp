#include "ICoreSystem.h"

namespace fs = boost::filesystem;

std::shared_ptr<ICoreSystem> ICoreSystem::instance;

ICoreSystem::ICoreSystem(const fs::path& _rootDir)
	: rootDir(_rootDir),
	  soundManager(nullptr)
{}

ICoreSystem::ptr ICoreSystem::getInstance()
{
	return instance;
}

void ICoreSystem::destroy()
{
	instance.reset();
}

fs::path ICoreSystem::getRootDir() const
{
	return rootDir;
}

SoundManager* ICoreSystem::getSounds()
{
	if (!soundManager)
	{
		soundManager = new SoundManager();
	}

	return soundManager;
}

#include "ICoreSystem.h"

namespace fs = boost::filesystem;

ICoreSystem::ICoreSystem(const fs::path& _rootDir)
	: rootDir(_rootDir)
{}

fs::path ICoreSystem::getRootDir() const
{
	return rootDir;
}

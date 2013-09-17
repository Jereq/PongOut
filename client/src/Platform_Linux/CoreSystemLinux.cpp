#include "CoreSystemLinux.h"

CoreSystemLinux::CoreSystemLinux(const boost::filesystem::path& _rootDir)
	: rootDir(_rootDir),
	  graphics(_rootDir)
{
}

boost::filesystem::path CoreSystemLinux::getRootDir() const
{
	return rootDir;
}

double CoreSystemLinux::getTime() const
{
	return glfwGetTime();
}

bool CoreSystemLinux::windowIsClosing() const
{
	return graphics.windowIsClosing();
}

void CoreSystemLinux::pollEvents()
{
	glfwPollEvents();
}

GraphicsLinux* CoreSystemLinux::getGraphics()
{
	return &graphics;
}

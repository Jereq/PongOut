#include "CoreSystemLinux.h"

CoreSystemLinux::CoreSystemLinux(const boost::filesystem::path& _rootDir)
	: rootDir(_rootDir),
	  renderer(_rootDir)
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
	return renderer.windowIsClosing();
}

void CoreSystemLinux::pollEvents()
{
	glfwPollEvents();
}

RendererLinux* CoreSystemLinux::getRenderer()
{
	return &renderer;
}

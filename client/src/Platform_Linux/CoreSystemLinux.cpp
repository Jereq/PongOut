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

bool CoreSystemLinux::isKeyPress(unsigned short _key)
{
	return glfwGetKey(graphics.window, _key) == GLFW_PRESS;
}

bool CoreSystemLinux::isNewKeyPress(unsigned short _key)
{
	return glfwGetKey(graphics.window, _key) == GLFW_PRESS;
}

GraphicsLinux* CoreSystemLinux::getGraphics()
{
	return &graphics;
}

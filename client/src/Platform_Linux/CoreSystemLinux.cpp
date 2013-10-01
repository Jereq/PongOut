#include "CoreSystemLinux.h"
#include <iostream>

namespace fs = boost::filesystem;

void CoreSystemLinux::errorCallback(int _error, const char* _description)
{
	fprintf(stderr, "GLFW error %d: %s\n", _error, _description);
}

bool ICoreSystem::init(int _argc, char** _argv)
{
	fs::path fullPath(fs::initial_path<fs::path>());
	fullPath = fs::system_complete(fs::path(_argv[0]));

	if (!fs::exists(fullPath))
	{
		std::cout << "Error: Invalid argv[0], stop hacking!" << std::endl;
		return false;
	}

	instance.reset();
	instance.reset(new CoreSystemLinux(fullPath.parent_path()));

	return true;
}

CoreSystemLinux::CoreSystemLinux(const boost::filesystem::path& _rootDir)
	: ICoreSystem(_rootDir)
{
	glfwSetErrorCallback(errorCallback);
	if (!glfwInit())
	{
		fprintf(stderr, "Error initializing GLFW.\n");
		throw std::exception();
	}
}

CoreSystemLinux::~CoreSystemLinux()
{
	glfwTerminate();
}

double CoreSystemLinux::getTime() const
{
	return glfwGetTime();
}

bool CoreSystemLinux::windowIsClosing() const
{
	return graphics && graphics->windowIsClosing();
}

void CoreSystemLinux::pollEvents()
{
	glfwPollEvents();
}

bool CoreSystemLinux::isKeyPress(unsigned short _key)
{
	return glfwGetKey(graphics->window, _key) == GLFW_PRESS;
}

bool CoreSystemLinux::isNewKeyPress(unsigned short _key)
{
	return isKeyPress(_key);
}

IGraphics::ptr CoreSystemLinux::getGraphics()
{
	if (!graphics)
	{
		graphics.reset(new GraphicsLinux(getRootDir()));
	}

	return graphics;
}

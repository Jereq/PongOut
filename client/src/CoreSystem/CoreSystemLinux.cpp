#include "CoreSystemLinux.h"
#include <iostream>

namespace fs = boost::filesystem;

void CoreSystemLinux::errorCallback(int _error, const char* _description)
{
	fprintf(stderr, "GLFW error %d: %s\n", _error, _description);
}

bool CoreSystemLinux::openWindow()
{
	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_DEPTH_BITS, 32);
	glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GraphicsLinux::MAJOR_GL_VERSION);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GraphicsLinux::MINOR_GL_VERSION);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	const static size_t WIDTH = 1280;
	const static size_t HEIGHT = 720;

	window = glfwCreateWindow(WIDTH, HEIGHT, "PongOut", nullptr, nullptr);
	if (!window)
	{
		glfwDefaultWindowHints();
		window = glfwCreateWindow(WIDTH, HEIGHT, "PongOut", nullptr, nullptr);

		if (!window)
		{
			return false;
		}
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	return true;
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
	: ICoreSystem(_rootDir),
	  window(nullptr)
{
	glfwSetErrorCallback(errorCallback);
	if (!glfwInit())
	{
		fprintf(stderr, "Error initializing GLFW.\n");
		throw std::exception();
	}

	if (!openWindow())
	{
		fprintf(stderr, "Error creating window.\n");
		glfwTerminate();
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

IGraphics::ptr CoreSystemLinux::getGraphics()
{
	if (!graphics)
	{
		graphics.reset(new GraphicsLinux(getRootDir(), window));
	}

	return graphics;
}

IInput::ptr CoreSystemLinux::getInput()
{
	if (!input)
	{
		input.reset(new InputLinux());
		input->registerWindowForInput(window);
	}

	return input;
}


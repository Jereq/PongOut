#include "Graphics.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdio>

void Graphics::errorCallback(int error, const char* description)
{
	fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

bool Graphics::openWindow()
{
	glfwDefaultWindowHints();
	//glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
	//glfwWindowHint(GLFW_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_VERSION_MINOR, 0);
	//glfwOpenWindowHint(GLFW_, 8);
	//glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	const static size_t WIDTH = 800;
	const static size_t HEIGHT = 480;

	window = glfwCreateWindow(WIDTH, HEIGHT, "PongOut", nullptr, nullptr);
	if (!window)
	{
		return false;
	}

	glfwMakeContextCurrent(window);

	return true;
}

void Graphics::printReport()
{
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	fprintf(stdout, "GL Vendor            : %s\n", vendor);
	fprintf(stdout, "GL Renderer          : %s\n", renderer);
	fprintf(stdout, "GL Version (string)  : %s\n", version);
	fprintf(stdout, "GL Version (integer) : %d.%d\n", major, minor);
	fprintf(stdout, "GLSL Version         : %s\n", glslVersion);

	//int versionMajor = glfwGetWindowParam(GLFW_OPENGL_VERSION_MAJOR);
	//int versionMinor = glfwGetWindowParam(GLFW_OPENGL_VERSION_MINOR);
	int forwardCompat = glfwGetWindowAttrib(window, GLFW_OPENGL_FORWARD_COMPAT);
	int profile = glfwGetWindowAttrib(window, GLFW_OPENGL_PROFILE);

	printf("\n");
	//printf("Actual version     : %d.%d\n", versionMajor, versionMinor);
	printf("Forward compatible : %s\n", (forwardCompat == GL_TRUE) ? "Yes" : "No");
	printf("Profile            : %s\n", (profile == GLFW_OPENGL_CORE_PROFILE) ? "Core" : (profile == GLFW_OPENGL_COMPAT_PROFILE) ? "Compatibility" : "Unknown");

	int glfwVersionMajor, glfwVersionMinor, glfwVersionRevision;
	glfwGetVersion(&glfwVersionMajor, &glfwVersionMinor, &glfwVersionRevision);
	printf("\n");
	printf("GLFW Version : %d.%d.%d\n", glfwVersionMajor, glfwVersionMinor, glfwVersionRevision);

	int glVersionMajor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
	int glVersionMinor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
	int glVersionRevision = glfwGetWindowAttrib(window, GLFW_CONTEXT_REVISION);
	printf("GL Version : %d.%d.%d\n", glVersionMajor, glVersionMinor, glVersionRevision);
}

Graphics::Graphics()
	: window(nullptr)
{
}

Graphics::~Graphics()
{
}

bool Graphics::init()
{
	glfwSetErrorCallback(errorCallback);
	if (!glfwInit())
	{
		fprintf(stderr, "Error initializing GLFW.\n");
		return false;
	}

	if (!openWindow())
	{
		fprintf(stderr, "Error creating window.\n");
		glfwTerminate();
		return false;
	}

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error initializing GLEW: %s\n", glewGetErrorString(err));
		glfwTerminate();
		return false;
	}

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	printReport();

	return true;
}

void Graphics::destroy()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

bool Graphics::windowClosing()
{
	return glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_ESCAPE);
}

double Graphics::getTime()
{
	return glfwGetTime();
}

void Graphics::pollEvents()
{
	glfwPollEvents();
}

void Graphics::swapBuffers()
{
	glfwSwapBuffers(window);
}

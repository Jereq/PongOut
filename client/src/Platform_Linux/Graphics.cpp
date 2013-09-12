#include "Graphics.h"

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <IL/il.h>

#include <cstdio>

const int Graphics::MAJOR_GL_VERSION = 4;
const int Graphics::MINOR_GL_VERSION = 0;

fs::path Graphics::rootDir;

void Graphics::errorCallback(int error, const char* description)
{
	fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

bool Graphics::openWindow()
{
	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_DEPTH_BITS, 32);
	glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MAJOR_GL_VERSION);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MINOR_GL_VERSION);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	const static size_t WIDTH = 800;
	const static size_t HEIGHT = 480;

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

bool Graphics::checkGLVersion(int majorRequiredVersion, int minorRequiredVersion)
{
	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	if (major != majorRequiredVersion)
	{
		return false;
	}

	if (minor < minorRequiredVersion)
	{
		return false;
	}

	return true;
}

void Graphics::initDevIL()
{
	ilInit();
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
}

void Graphics::printReport()
{
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

	fprintf(stdout, "GL Vendor    : %s\n", vendor);
	fprintf(stdout, "GL Renderer  : %s\n", renderer);
	fprintf(stdout, "GL Version   : %s\n", version);
	fprintf(stdout, "GLSL Version : %s\n", glslVersion);

	const char* glfwVersion = glfwGetVersionString();
	fprintf(stdout, "GLFW Version : %s\n", glfwVersion);
}

bool Graphics::loadRectangleShader()
{
	static fs::path vertexShaderPath(rootDir / "shaders/basicRect.vert");
	static fs::path fragmentShaderPath(rootDir / "shaders/basicRect.frag");

	if (!rectangleProgram.compileShaderFromFile(vertexShaderPath.string(), GLSLShaderType::VERTEX) ||
			!rectangleProgram.compileShaderFromFile(fragmentShaderPath.string(), GLSLShaderType::FRAGMENT))
	{
		fprintf(stderr, "%s", rectangleProgram.log().c_str());
		return false;
	}

	return true;
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

	if (!checkGLVersion(MAJOR_GL_VERSION, MINOR_GL_VERSION))
	{
		fprintf(stderr, "Error: Required OpenGL version not found.\n");
		fprintf(stderr, "Error: This application requires at least version %d.%d.\n", MAJOR_GL_VERSION, MINOR_GL_VERSION);
		fprintf(stderr, "Error: Please update your graphics drivers and try again.\n");
		glfwTerminate();
		return false;
	}

	glEnable(GL_DEPTH_TEST);

	initDevIL();

	printReport();

	if (!loadRectangleShader())
	{
		glfwTerminate();
		return false;
	}

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

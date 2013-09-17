#include "GraphicsLinux.h"

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <IL/il.h>

#include <cstdio>

const int GraphicsLinux::MAJOR_GL_VERSION = 4;
const int GraphicsLinux::MINOR_GL_VERSION = 0;

void GraphicsLinux::errorCallback(int _error, const char* _description)
{
	fprintf(stderr, "GLFW error %d: %s\n", _error, _description);
}

bool GraphicsLinux::openWindow()
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

bool GraphicsLinux::checkGLVersion(int _majorRequiredVersion, int _minorRequiredVersion)
{
	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	if (major != _majorRequiredVersion)
	{
		return false;
	}

	if (minor < _minorRequiredVersion)
	{
		return false;
	}

	return true;
}

void GraphicsLinux::initDevIL()
{
	ilInit();
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
}

void GraphicsLinux::printReport()
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

bool GraphicsLinux::loadRectangleShader()
{
	static fs::path vertexShaderPath(rootDir / "shaders/basicRect.vert");
	static fs::path fragmentShaderPath(rootDir / "shaders/basicRect.frag");

	if (!rectangleProgram.compileShaderFromFile(vertexShaderPath.string(), GLSLShaderType::VERTEX) ||
			!rectangleProgram.compileShaderFromFile(fragmentShaderPath.string(), GLSLShaderType::FRAGMENT))
	{
		fprintf(stderr, "%s", rectangleProgram.log().c_str());
		return false;
	}

	rectangleProgram.bindAttribLocation(0, "vertexPosition");
	rectangleProgram.bindAttribLocation(1, "vertexTexCoord");

	if (!rectangleProgram.link())
	{
		fprintf(stderr, "%s", rectangleProgram.log().c_str());
		return false;
	}

	rectangleProgram.printActiveAttribs();
	rectangleProgram.printActiveUniforms();

	rectangleProgram.use();

	return true;
}

void GraphicsLinux::initRectMesh()
{
	glGenBuffers(2, rectVboHandles);
	GLuint positionBufferHandle = rectVboHandles[0];
	GLuint texCoordBufferHandle = rectVboHandles[1];

	const static glm::vec3 positions[6] = {
			glm::vec3(-1.f,  1.f, 0.f),
			glm::vec3(-1.f, -1.f, 0.f),
			glm::vec3( 1.f,  1.f, 0.f),

			glm::vec3( 1.f,  1.f, 0.f),
			glm::vec3(-1.f, -1.f, 0.f),
			glm::vec3( 1.f, -1.f, 0.f)
	};

	const static glm::vec2 texCoords[6] = {
			glm::vec2(0.f, 1.f),
			glm::vec2(0.f, 0.f),
			glm::vec2(1.f, 1.f),

			glm::vec2(1.f, 1.f),
			glm::vec2(0.f, 0.f),
			glm::vec2(1.f, 0.f)
	};

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);

	glGenVertexArrays(1, &rectVaoHandle);
	glBindVertexArray(rectVaoHandle);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferHandle);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
}

GraphicsLinux::GraphicsLinux(const fs::path& _rootDir)
	: window(nullptr),
	  rectVaoHandle(0),
	  rootDir(_rootDir)
{
}

bool GraphicsLinux::init()
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

	initRectMesh();

	projectionMatrix = glm::ortho(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f);

	glClearColor(1.f, 0.f, 1.f, 1.f);

	return true;
}

void GraphicsLinux::destroy()
{
	if (!loadedTextureIds.empty())
	{
		glDeleteTextures(loadedTextureIds.size(), loadedTextureIds.data());
		loadedTextureIds.clear();
	}

	rectangleProgram.free();

	glDeleteVertexArrays(1, &rectVaoHandle);
	glDeleteBuffers(2, rectVboHandles);

	glfwDestroyWindow(window);
	glfwTerminate();
}

bool GraphicsLinux::loadImage(const fs::path& _imagePath)
{
	ILuint image = ilGenImage();
	ilBindImage(image);

	if (!ilLoad(IL_PNG, _imagePath.string().c_str()))
	{
		fprintf(stderr, "Failed to load \"%s\"\n", _imagePath.string().c_str());
	}

	GLenum textureUnit = GL_TEXTURE0 + loadedTextureIds.size();

	GLuint tId;
	glGenTextures(1, &tId);
	loadedTextureIds.push_back(tId);

	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, tId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_TYPE), ilGetData());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	ilDeleteImage(image);
}

void GraphicsLinux::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	rectangleProgram.use();
	rectangleProgram.setUniform("projectionMatrix", projectionMatrix);
	rectangleProgram.setUniform("tex", 0);

	glBindVertexArray(rectVaoHandle);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glfwSwapBuffers(window);
}

bool GraphicsLinux::windowIsClosing() const
{
	return glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_ESCAPE);
}

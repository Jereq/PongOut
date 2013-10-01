#include "GraphicsLinux.h"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
namespace fs = boost::filesystem;

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <IL/il.h>

#include <cstdio>
#include <sstream>
#include <stdexcept>

const int GraphicsLinux::MAJOR_GL_VERSION = 4;
const int GraphicsLinux::MINOR_GL_VERSION = 0;

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
	static fs::path geometryShaderPath(rootDir / "shaders/basicRect.geom");
	static fs::path fragmentShaderPath(rootDir / "shaders/basicRect.frag");

	if (!rectangleProgram.compileShaderFromFile(vertexShaderPath.string(), GLSLShaderType::VERTEX)
			|| !rectangleProgram.compileShaderFromFile(geometryShaderPath.string(), GLSLShaderType::GEOMETRY)
			|| !rectangleProgram.compileShaderFromFile(fragmentShaderPath.string(), GLSLShaderType::FRAGMENT))
	{
		fprintf(stderr, "%s", rectangleProgram.log().c_str());
		return false;
	}

	rectangleProgram.bindAttribLocation(0, "vertexPosition");
	rectangleProgram.bindAttribLocation(1, "vertexSize");

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

void GraphicsLinux::initRectBuffer()
{
	glGenBuffers(1, &rectVboHandle);

	glGenVertexArrays(1, &rectVaoHandle);
	glBindVertexArray(rectVaoHandle);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, rectVboHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Rectangle), (const GLvoid*) offsetof(Rectangle, position));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Rectangle), (const GLvoid*) offsetof(Rectangle, size));
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Rectangle), (const GLvoid*) offsetof(Rectangle, rotation));
}

void GraphicsLinux::bufferData(const recs_t& _rects)
{
	glBindBuffer(GL_ARRAY_BUFFER, rectVboHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(recs_t) * _rects.size(), _rects.data(), GL_DYNAMIC_DRAW);
}

GraphicsLinux::GraphicsLinux(const fs::path& _rootDir)
	: window(nullptr),
	  rectVaoHandle(0),
	  rectVboHandle(0),
	  rootDir(_rootDir)
{
}

bool GraphicsLinux::init()
{
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
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	initDevIL();

	printReport();

	if (!loadRectangleShader())
	{
		glfwTerminate();
		return false;
	}

	initRectBuffer();

	projectionMatrix = glm::ortho(-1.f, 1.f, -1.f, 1.f, 1.f, -1.f);

	glClearColor(1.f, 0.f, 1.f, 1.f);

	return true;
}

void GraphicsLinux::destroy()
{
	if (!loadedTextures.empty())
	{
		for (auto& image : loadedTextures)
		{
			glDeleteTextures(1, &image.second.textureID);
			ilDeleteImage(image.second.id);

		}
		loadedTextures.clear();
	}

	rectangleProgram.free();

	glDeleteVertexArrays(1, &rectVaoHandle);
	glDeleteBuffers(1, &rectVboHandle);

	glfwDestroyWindow(window);
	glfwTerminate();
}

bool GraphicsLinux::loadResources(const boost::filesystem::path& _resourceDir)
{
	if (!fs::exists(_resourceDir) || !fs::is_directory(_resourceDir))
	{
		return false;
	}

	fs::path resourceFile = _resourceDir / "resources.txt";
	if (!fs::exists(resourceFile) || !fs::is_regular_file(resourceFile))
	{
		return false;
	}

	int rowNr = 0;
	fs::fstream f(resourceFile);
	while (!f.eof())
	{
		rowNr++;

		std::string line;
		std::getline(f, line);

		std::istringstream ss(line);
		std::string type;
		std::getline(ss, type, ':');

		if (ss.eof() || type != "texture")
		{
			continue;
		}

		std::string resourceName;
		std::string resourcePath;
		ss >> resourceName >> resourcePath;

		if (resourceName.empty() || resourcePath.empty())
		{
			continue;
		}

		LoadedImage image = loadImage(_resourceDir / resourcePath);
		if (image.textureID)
		{
			loadedTextures.insert(std::pair<std::string, LoadedImage>(resourceName, image));
		}
	}

	return true;
}

GraphicsLinux::LoadedImage GraphicsLinux::loadImage(const fs::path& _imagePath)
{
	LoadedImage res{0, _imagePath, 0};
	res.id = ilGenImage();
	ilBindImage(res.id);

	if (!ilLoadImage(_imagePath.string().c_str()))
	{
		fprintf(stderr, "Failed to load \"%s\"\n", _imagePath.string().c_str());
		return res;
	}

	GLuint tId;
	glGenTextures(1, &tId);

	glBindTexture(GL_TEXTURE_2D, tId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_TYPE), ilGetData());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	res.textureID = tId;
	return res;
}

void GraphicsLinux::addRectangle(glm::vec3 _center, glm::vec2 _size, float _rotation, std::string _id)
{
	if (loadedTextures.count(_id) == 0)
	{
		_id = "textureNotFound";
	}

	Rectangle rect{
		_center,
		_size,
		_rotation
	};

	registeredRectangles[_id].push_back(rect);
}

void GraphicsLinux::drawFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (!registeredRectangles.empty())
	{
		rectangleProgram.use();
		rectangleProgram.setUniform("projectionMatrix", projectionMatrix);
		rectangleProgram.setUniform("tex", 0);
		glActiveTexture(GL_TEXTURE0);

		glBindVertexArray(rectVaoHandle);

		for (auto& recList : registeredRectangles)
		{
			if (recList.second.empty())
			{
				continue;
			}

			bufferData(recList.second);

			glBindTexture(GL_TEXTURE_2D, loadedTextures.at(recList.first).textureID);
			glDrawArrays(GL_POINTS, 0, recList.second.size());

			recList.second.clear();
		}
	}

	glfwSwapBuffers(window);
}

bool GraphicsLinux::windowIsClosing() const
{
	return glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_ESCAPE);
}

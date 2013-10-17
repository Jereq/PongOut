#include "GraphicsLinux.h"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
namespace fs = boost::filesystem;

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <IL/il.h>

#include <utf8.h>

#include <ResourceLoader/ResourceLoader.h>
#include <Input/InputLinux.h>

#include <cstdio>
#include <iostream>
#include <sstream>
#include <stdexcept>

const int GraphicsLinux::MAJOR_GL_VERSION = 4;
const int GraphicsLinux::MINOR_GL_VERSION = 0;

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

	return true;
}

bool GraphicsLinux::loadTextShader()
{
	static fs::path vertexShaderPath(rootDir / "shaders/basicRect.vert");
	static fs::path geometryShaderPath(rootDir / "shaders/basicRect.geom");
	static fs::path fragmentShaderPath(rootDir / "shaders/basicText.frag");

	if (!textProgram.compileShaderFromFile(vertexShaderPath.string(), GLSLShaderType::VERTEX)
			|| !textProgram.compileShaderFromFile(geometryShaderPath.string(), GLSLShaderType::GEOMETRY)
			|| !textProgram.compileShaderFromFile(fragmentShaderPath.string(), GLSLShaderType::FRAGMENT))
	{
		fprintf(stderr, "%s", textProgram.log().c_str());
		return false;
	}

	textProgram.bindAttribLocation(0, "vertexPosition");
	textProgram.bindAttribLocation(1, "vertexSize");

	if (!textProgram.link())
	{
		fprintf(stderr, "%s", textProgram.log().c_str());
		return false;
	}

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(Rectangle) * _rects.size(), _rects.data(), GL_DYNAMIC_DRAW);
}

GraphicsLinux::GraphicsLinux(const fs::path& _rootDir, GLFWwindow* _window)
	: window(_window),
	  rectVaoHandle(0),
	  rectVboHandle(0),
	  rootDir(_rootDir)
{
}

bool GraphicsLinux::init()
{
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
		return false;
	}

	if (!loadTextShader())
	{
		return false;
	}

	initRectBuffer();

	projectionMatrix = glm::ortho(-1.f, 1.f, -1.f, 1.f, 1.f, -1.f);

	glClearColor(1.f, 0.f, 1.f, 1.f);

	Font::ErrorCode fErr = menuFont.init("/usr/share/fonts/truetype/freefont/FreeSans.ttf", 24);
	if (fErr != Font::ErrorCode::OK)
	{
		return false;
	}

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

	if (!loadedChars.empty())
	{
		for (auto& image : loadedChars)
		{
			glDeleteTextures(1, &image.second.textureID);
		}
		loadedChars.clear();
	}

	glDeleteVertexArrays(1, &rectVaoHandle);
	glDeleteBuffers(1, &rectVboHandle);

	glfwDestroyWindow(window);
	glfwTerminate();
}

bool GraphicsLinux::loadResources(const boost::filesystem::path& _resourceDir)
{
	std::vector<ResourceLoader::Resource> textureResources;
	ResourceLoader::ErrorCode err = ResourceLoader::getResources(textureResources, _resourceDir, "texture");
	if (err == ResourceLoader::ErrorCode::INVALID_FORMAT)
	{
		std::cout << "Warning: " << _resourceDir / "resources.txt" << " contains invalid formatting." << std::endl;
	}
	else if (err != ResourceLoader::ErrorCode::SUCCESS)
	{
		std::cout << "Error: Could not load resources from" << _resourceDir << std::endl;
		return false;
	}

	for (auto tex : textureResources)
	{
		LoadedImage image = loadImage(tex.path);
		if (image.textureID)
		{
			loadedTextures.insert(std::pair<std::string, LoadedImage>(tex.name, image));
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

GraphicsLinux::ErrorCode GraphicsLinux::loadChar(LoadedChar& _charOut, char32_t _character)
{
	Font::Glyph glyph;
	Font::ErrorCode err = menuFont.getGlyph(glyph, _character);
	if (err != Font::ErrorCode::OK)
	{
		return ErrorCode::GLYPH_COULD_NOT_BE_LOADED;
	}

	GLint prevAlignment;
	glGetIntegerv(GL_UNPACK_ALIGNMENT, &prevAlignment);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &_charOut.textureID);
	glBindTexture(GL_TEXTURE_2D, _charOut.textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, glyph.width, glyph.height, 0, GL_RED, GL_UNSIGNED_BYTE, glyph.bitBuffer.data());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glPixelStorei(GL_UNPACK_ALIGNMENT, prevAlignment);

	_charOut.origin = glyph.origin;
	_charOut.advance = glyph.advance;
	_charOut.size = glm::vec2(glyph.width, glyph.height);

	return ErrorCode::OK;
}

GraphicsLinux::ErrorCode GraphicsLinux::getChar(LoadedChar& _charOut, char32_t _character)
{
	auto it = loadedChars.find(_character);
	if (it == loadedChars.end())
	{
		LoadedChar c;
		if (loadChar(c, _character) != ErrorCode::OK)
		{
			return ErrorCode::GLYPH_COULD_NOT_BE_LOADED;
		}

		auto resPair = loadedChars.insert(std::make_pair(_character, c));
		it = resPair.first;
	}

	_charOut = it->second;

	return ErrorCode::OK;
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

IGraphics::ErrorCode GraphicsLinux::addText(glm::vec3 _startPos, glm::vec2 _letterSize, const std::string& _text)
{
	auto utf8It = _text.begin();
	auto itEnd = _text.end();

	if (!utf8::is_valid(utf8It, itEnd))
	{
		return ErrorCode::INVALID_ARGUMENT;
	}

	glm::vec3 penPos = _startPos;

	while (utf8It != itEnd)
	{
		char32_t character32 = utf8::next(utf8It, itEnd);

		LoadedChar c;
		ErrorCode err = getChar(c, character32);
		if (err != ErrorCode::OK)
		{
			return err;
		}

		float scale = 1.f / 24.f;
		glm::vec3 posScale(_letterSize.x * scale, _letterSize.y * scale, 1.f);
		glm::vec2 size = _letterSize * c.size * scale;

		Rectangle rect{
			penPos + glm::vec3(c.origin.x, -c.size.y + c.origin.y, 0.f) * posScale + glm::vec3(size, 0.f) * 0.5f,
			size,
			0.f
		};

		registeredCharacters[c.textureID].push_back(rect);

		penPos += glm::vec3(c.advance.x, c.advance.y, 0.f) * posScale;
	}

	return ErrorCode::OK;
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

	if (!registeredCharacters.empty())
	{
		textProgram.use();
		textProgram.setUniform("projectionMatrix", projectionMatrix);
		textProgram.setUniform("tex", 0);
		glActiveTexture(GL_TEXTURE0);

		glBindBuffer(GL_ARRAY_BUFFER, rectVboHandle);

		for (auto& recList : registeredCharacters)
		{
			if (recList.second.empty())
			{
				continue;
			}

			bufferData(recList.second);

			glBindTexture(GL_TEXTURE_2D, recList.first);
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

#pragma once

#include "Font.h"
#include "GLSLProgram.h"
#include "GraphicsLinux.h"
#include "IGraphics.h"

#include <boost/filesystem.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <IL/il.h>

#include <map>

class GraphicsLinux : public IGraphics
{
public:

private:
	GLFWwindow* window;
	GLSLProgram rectangleProgram;
	GLSLProgram textProgram;
	GLuint rectVaoHandle;
	GLuint rectVboHandle;

	glm::mat4 projectionMatrix;

	boost::filesystem::path rootDir;

	struct LoadedImage
	{
		ILuint id;
		boost::filesystem::path path;
		GLuint textureID;
	};

	std::map<std::string, LoadedImage> loadedTextures;

	struct LoadedChar
	{
		GLuint textureID;
		glm::vec2 origin;
		glm::vec2 advance;
		glm::vec2 size;
	};

	std::map<char32_t, LoadedChar> loadedChars;
	Font menuFont;

	struct Rectangle
	{
		glm::vec3 position;
		glm::vec2 size;
		float rotation;
	};
	typedef std::vector<Rectangle> recs_t;
	std::map<std::string, recs_t> registeredRectangles;
	std::map<GLuint, recs_t> registeredCharacters;

	bool checkGLVersion(int _majorRequiredVersion, int _minorRequiredVersion);
	void initDevIL();
	void printReport();
	bool loadRectangleShader();
	bool loadTextShader();
	void initRectBuffer();

	LoadedImage loadImage(const boost::filesystem::path& _imagePath);

	ErrorCode loadChar(LoadedChar& _charOut, char32_t _character);
	ErrorCode getChar(LoadedChar& _charOut, char32_t _character);

	void bufferData(const recs_t& _rects);

public:
	typedef std::shared_ptr<GraphicsLinux> ptr;

	const static int MAJOR_GL_VERSION;
	const static int MINOR_GL_VERSION;

	GraphicsLinux(const boost::filesystem::path& _rootDir, GLFWwindow* _window);

	virtual bool init() override;
	virtual void destroy() override;

	virtual bool loadResources(const boost::filesystem::path& _resourceDir) override;
	virtual void addRectangle(glm::vec3 _center, glm::vec2 _size, float _rotation, std::string _id) override;
	virtual ErrorCode addText(glm::vec3 _startPos, glm::vec2 _letterSize, const std::string& _text) override;

	virtual void drawFrame() override;

	bool windowIsClosing() const;
};

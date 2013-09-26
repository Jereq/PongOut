#pragma once


#include "GLSLProgram.h"
#include "GraphicsLinux.h"
#include "../IGraphics.h"

#include <boost/filesystem.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <IL/il.h>

#include <map>

class GraphicsLinux : public IGraphics
{
private:
	const static int MAJOR_GL_VERSION;
	const static int MINOR_GL_VERSION;

	GLFWwindow* window;
	GLSLProgram rectangleProgram;
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

	struct Rectangle
	{
		glm::vec3 position;
		glm::vec2 size;
		float rotation;
	};
	typedef std::vector<Rectangle> recs_t;
	std::map<std::string, recs_t> registeredRectangles;

	bool openWindow();
	bool checkGLVersion(int _majorRequiredVersion, int _minorRequiredVersion);
	void initDevIL();
	void printReport();
	bool loadRectangleShader();
	void initRectBuffer();

	LoadedImage loadImage(const boost::filesystem::path& _imagePath);

	void bufferData(const recs_t& _rects);

public:
	typedef std::shared_ptr<GraphicsLinux> ptr;

	GraphicsLinux(const boost::filesystem::path& _rootDir);

	virtual bool init() override;
	virtual void destroy() override;

	virtual bool loadResources(const boost::filesystem::path& _resourceDir) override;
	virtual void addRectangle(glm::vec3 _center, glm::vec2 _size, float _rotation, std::string _id) override;

	virtual void drawFrame() override;

	bool windowIsClosing() const;
};

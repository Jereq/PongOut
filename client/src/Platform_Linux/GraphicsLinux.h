#pragma once


#include "GLSLProgram.h"
#include "GraphicsLinux.h"
#include "../IGraphics.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <boost/filesystem.hpp>

class GraphicsLinux : public IGraphics
{
private:
	const static int MAJOR_GL_VERSION;
	const static int MINOR_GL_VERSION;

	GLFWwindow* window;
	GLSLProgram rectangleProgram;
	GLuint rectVaoHandle;
	GLuint rectVboHandles[2];

	std::vector<GLuint> loadedTextureIds;

	glm::mat4 projectionMatrix;

	boost::filesystem::path rootDir;

	static void errorCallback(int _error, const char* _description);

	bool openWindow();
	bool checkGLVersion(int _majorRequiredVersion, int _minorRequiredVersion);
	void initDevIL();
	void printReport();
	bool loadRectangleShader();
	void initRectMesh();

public:
	GraphicsLinux(const boost::filesystem::path& _rootDir);

	virtual bool init() override;
	virtual void destroy() override;

	virtual bool loadResources(const boost::filesystem::path& _resourceDir) override;
	virtual bool loadImage(const boost::filesystem::path& _imagePath) override;
	virtual void addRectangle(glm::vec2 _center, glm::vec2 _size, float _rotation, std::string id) override;

	virtual void drawFrame() override;

	bool windowIsClosing() const;
};

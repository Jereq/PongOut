#pragma once

#include <boost/filesystem.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GLSLProgram.h"

class Graphics
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

	static void errorCallback(int error, const char* description);

	bool openWindow();
	bool checkGLVersion(int majorRequiredVersion, int minorRequiredVersion);
	void initDevIL();
	void printReport();
	bool loadRectangleShader();
	void initRectMesh();

public:
	Graphics(const boost::filesystem::path& _rootDir);
	virtual ~Graphics();

	bool init();
	void destroy();

	bool loadImage(const boost::filesystem::path& file);

	bool windowClosing() const;
	double getTime();
	void pollEvents();

	void draw();
};

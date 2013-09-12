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

	static void errorCallback(int error, const char* description);

	bool openWindow();
	bool checkGLVersion(int majorRequiredVersion, int minorRequiredVersion);
	void initDevIL();
	void printReport();
	bool loadRectangleShader();

public:
	static boost::filesystem::path rootDir;

	Graphics();
	virtual ~Graphics();

	bool init();
	void destroy();
	bool windowClosing();

	double getTime();
	void pollEvents();
	void swapBuffers();
};

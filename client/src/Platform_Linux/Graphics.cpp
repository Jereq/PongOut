#include "Graphics.h"

#include <GL/glew.h>

#include <cstdio>

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
}

Graphics::Graphics()
{
}

Graphics::~Graphics()
{
}

bool Graphics::init()
{
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error initializing GLEW: %s\n", glewGetErrorString(err));
		return false;
	}

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	printReport();

	return true;
}

void Graphics::destroy()
{
}

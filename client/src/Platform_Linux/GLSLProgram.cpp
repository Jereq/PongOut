#include "GLSLProgram.h"

#include <cerrno>
#include <cstdio>
#include <cstring>

GLint GLSLProgram::getUniformLocation(const std::string& name)
{
	return glGetUniformLocation(handle, name.c_str());
}

GLSLProgram::GLSLProgram()
	: handle(0)
	, linked(false)
{}

GLSLProgram::~GLSLProgram()
{
	free();
}

bool GLSLProgram::compileShaderFromFile(const std::string& fileName, GLSLShaderType type)
{
	FILE* f = fopen(fileName.c_str(), "rb");

	if (f == nullptr)
	{
		logString = "Error opening shader file: ";
		logString.append(strerror(errno)).append("\n");
		return false;
	}

	fseek(f, 0, SEEK_END);
	if (ferror(f))
	{
		logString = "Error seeking in shader file.\n";
		fclose(f);
		return false;
	}

	size_t size = ftell(f);
	if (size == -1L)
	{
		logString = "Error getting shader file size: ";
		logString.append(strerror(errno)).append("\n");
		fclose(f);
		return false;
	}

	GLchar* data = new GLchar[size + 1];
	data[size] = '\0';

	rewind(f);
	if (fread(data, sizeof(GLchar), size, f) != size)
	{
		if (ferror(f))
		{
			logString = "Error reading shader file: ";
		}
		else if (feof(f))
		{
			logString = "End of file reached unexpectedly: ";
		}
		else
		{
			logString = "Unknown error caused an incorrect read: ";
		}

		logString.append(strerror(errno)).append("\n");

		fclose(f);
		delete[] data;
		return false;
	}

	fclose(f);

	const std::string& shaderSource(data);
	delete[] data;

	return compileShaderFromString(shaderSource, type);
}

bool GLSLProgram::compileShaderFromString(const std::string& source, GLSLShaderType type)
{
	GLenum shaderType;
	switch (type)
	{
	case GLSLShaderType::VERTEX:
	case GLSLShaderType::FRAGMENT:
	case GLSLShaderType::GEOMETRY:
	case GLSLShaderType::TESS_CONTROL:
	case GLSLShaderType::TESS_EVALUATION:
		shaderType = (GLenum)type;
		break;

	default:
		logString = "Unknown shader type\n";
		return false;
	}

	GLuint shader = glCreateShader(shaderType);
	if (0 == shader)
	{
		logString = "Error creating shader\n";
		return false;
	}

	const GLchar* shaderSource = source.c_str();
	glShaderSource(shader, 1, &shaderSource, nullptr);

	glCompileShader(shader);

	GLint res;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &res);
	if (res == GL_FALSE)
	{
		logString = "Shader compilation failed:\n";
	}
	else
	{
		logString = "Shader compilation succeeded:\n";
	}

	GLint logLen;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
	if (logLen > 0)
	{
		char* log = new char[logLen];
		glGetShaderInfoLog(shader, logLen, nullptr, log);

		logString.append(log);

		delete[] log;
	}

	if (res == GL_FALSE)
	{
		glDeleteShader(shader);
		return false;
	}

	if (!glIsProgram(handle))
	{
		handle = glCreateProgram();
		if (handle == 0)
		{
			logString.append("Error creating program\n");
			glDeleteShader(shader);
			return false;
		}
	}

	glAttachShader(handle, shader);
	glDeleteShader(shader);

	return true;
}

bool GLSLProgram::link()
{
	if (!glIsProgram(handle))
	{
		logString = "Failed to link program: Program does not exist.\n";
		return false;
	}

	glLinkProgram(handle);
	GLint status;
	glGetProgramiv(handle, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		logString = "Failed to link shader program!\n";
		linked = false;
	}
	else
	{
		logString = "Successfully linked shader program.\n";
		linked = true;
	}

	GLint logLen;
	glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &logLen);
	if (logLen > 0)
	{
		char* log = new char[logLen];
		glGetProgramInfoLog(handle, logLen, nullptr, log);

		logString.append(log);

		delete[] log;
	}

	return linked;
}

void GLSLProgram::use()
{
	glUseProgram(handle);
}

bool GLSLProgram::isInUse()
{
	GLint currentProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
	return currentProgram == handle;
}

void GLSLProgram::free()
{
	if (glIsProgram(handle))
	{
		glDeleteProgram(handle);
	}

	handle = 0;
	linked = false;
	logString = "";
}

std::string GLSLProgram::log() const
{
	return logString;
}

GLuint GLSLProgram::getHandle() const
{
	return handle;
}

bool GLSLProgram::isLinked() const
{
	return linked;
}

void GLSLProgram::bindAttribLocation(GLuint location, const std::string& name)
{
	glBindAttribLocation(handle, location, name.c_str());
}

void GLSLProgram::bindFragDataLocation(GLuint location, const std::string& name)
{
	if (isInUse())
	{
		glBindFragDataLocation(handle, location, name.c_str());
	}
	else
	{
		fprintf(stderr, "Attempting to set uniform on wrong program!\n");
	}
}

void GLSLProgram::setUniform(const std::string& name, bool val)
{
	if (isInUse())
	{
		glUniform1i(getUniformLocation(name), val);
	}
	else
	{
		fprintf(stderr, "Attempting to set uniform on wrong program!\n");
	}
}

void GLSLProgram::setUniform(const std::string& name, float val)
{
	if (isInUse())
	{
		glUniform1f(getUniformLocation(name), val);
	}
	else
	{
		fprintf(stderr, "Attempting to set uniform on wrong program!\n");
	}
}

void GLSLProgram::setUniform(const std::string& name, int val)
{
	if (isInUse())
	{
		glUniform1i(getUniformLocation(name), val);
	}
	else
	{
		fprintf(stderr, "Attempting to set uniform on wrong program!\n");
	}
}

void GLSLProgram::setUniform(const std::string& name, float x, float y, float z)
{
	if (isInUse())
	{
		glUniform3f(getUniformLocation(name), x, y, z);
	}
	else
	{
		fprintf(stderr, "Attempting to set uniform on wrong program!\n");
	}
}

void GLSLProgram::setUniform(const std::string& name, const glm::vec3& v)
{
	if (isInUse())
	{
		glUniform3fv(getUniformLocation(name), 1, &v[0]);
	}
	else
	{
		fprintf(stderr, "Attempting to set uniform on wrong program!\n");
	}
}

void GLSLProgram::setUniform(const std::string& name, const glm::vec4& v)
{
	if (isInUse())
	{
		glUniform4fv(getUniformLocation(name), 1, &v[0]);
	}
	else
	{
		fprintf(stderr, "Attempting to set uniform on wrong program!\n");
	}
}

void GLSLProgram::setUniform(const std::string& name, const glm::mat3& m)
{
	if (isInUse())
	{
		glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, &m[0][0]);
	}
	else
	{
		fprintf(stderr, "Attempting to set uniform on wrong program!\n");
	}
}

void GLSLProgram::setUniform(const std::string& name, const glm::mat4& m)
{
	if (isInUse())
	{
		glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &m[0][0]);
	}
	else
	{
		fprintf(stderr, "Attempting to set uniform on wrong program!\n");
	}
}

void GLSLProgram::printActiveUniforms()
{
	GLint maxLength, nUniforms;
	glGetProgramiv(handle, GL_ACTIVE_UNIFORMS, &nUniforms);
	glGetProgramiv(handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength);

	GLchar* name = new GLchar[maxLength];

	printf(" Location | Name\n");
	printf("---------------------------------------------------------\n");
	for (int i = 0; i < nUniforms; i++)
	{
		GLint size;
		GLenum type;

		glGetActiveUniform(handle, i, maxLength, NULL, &size, &type, name);
		GLint location = glGetUniformLocation(handle, name);
		printf(" %-8d | %s\n", location, name);
	}

	delete[] name;
}

void GLSLProgram::printActiveAttribs()
{
	GLint maxLength, nAttribs;
	glGetProgramiv(handle, GL_ACTIVE_ATTRIBUTES, &nAttribs);
	glGetProgramiv(handle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);

	GLchar* name = new GLchar[maxLength];

	printf("\n Index | Name\n");
	printf("---------------------------------------------------------\n");
	for (int i = 0; i < nAttribs; i++)
	{
		GLint size;
		GLenum type;

		glGetActiveAttrib(handle, i, maxLength, NULL, &size, &type, name);
		GLint location = glGetAttribLocation(handle, name);
		printf(" %-5d | %s\n", location, name);
	}

	delete[] name;
}

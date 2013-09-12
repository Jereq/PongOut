#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <memory>
#include <string>

/*
 * The different shader types that can be compiled and attached to a glsl program.
 */
enum class GLSLShaderType
{
	VERTEX = GL_VERTEX_SHADER,
	FRAGMENT = GL_FRAGMENT_SHADER,
	GEOMETRY = GL_GEOMETRY_SHADER,
	TESS_CONTROL = GL_TESS_CONTROL_SHADER,
	TESS_EVALUATION = GL_TESS_EVALUATION_SHADER
};

/*
 * Resresentation of a glsl program, allowing shaders to be compiled and linked.
 */
class GLSLProgram
{
private:
	GLuint handle;
	bool linked;
	std::string logString;

	/*
	 * Returns the location of a uniform in a compiled program.
	 */
	GLint getUniformLocation(const std::string& name);

public:
	/*
	 * Shared pointer to be used when passing the program around or sharing it.
	 */
	typedef std::shared_ptr<GLSLProgram> ptr;

	/*
	 * constructor.
	 */
	GLSLProgram();

	/*
	 * destructor. Not to be called explicitly.
	 */
	~GLSLProgram();

	/*
	 * Compiles a shader from a file and attaches it to the program.
	 * Returns true on success, otherwise false.
	 * If not successful, any error can be retreived using log().
	 */
	bool compileShaderFromFile(const std::string& fileName, GLSLShaderType type);

	/*
	 * Compiles a shader from a string and attaches it to the program.
	 * Returns true on success, otherwise false.
	 * If not successful, any error can be retreived using log().
	 */
	bool compileShaderFromString(const std::string& source, GLSLShaderType type);

	/*
	 * Links any attached shaders into an executable glsl program.
	 * Returns true on success, otherwise false.
	 * If not successful, any error can be retreived using log().
	 */
	bool link();

	/*
	 * Sets this program to be the active one.
	 */
	void use();

	/*
	 * Return true if the program is currently in use.
	 */
	bool isInUse();

	/*
	 * Frees any resources held by this program. Invalidates the handle.
	 */
	void free();

	/*
	 * Returns the log for the last operation.
	 */
	std::string log() const;

	/*
	 * Returns the openGL handle for the program represented by this object.
	 * The program should not be modified outside this class,
	 * to release any resources held by this program, use free().
	 */
	GLuint getHandle() const;

	/*
	 * Returns whether or not the program has been successfully linked.
	 * The program can not be used until it has been successfully linked.
	 */
	bool isLinked() const;

	/*
	 * Binds a attribute to the specified location.
	 * The program must be linked for any changes to take effect.
	 */
	void bindAttribLocation(GLuint location, const std::string& name);

	/*
	 * Binds a fragment data output to the specified location.
	 * The program must be linked for any changes to take effect.
	 */
	void bindFragDataLocation(GLuint location, const std::string& name);

	/*
	 * Set the uniform specified by name to the value val.
	 * The program must be in use.
	 */
	void setUniform(const std::string& name, bool val);
	/*
	 * Set the uniform specified by name to the value val.
	 * The program must be in use.
	 */
	void setUniform(const std::string& name, float val);
	/*
	 * Set the uniform specified by name to the value val.
	 * The program must be in use.
	 */
	void setUniform(const std::string& name, int val);
	/*
	 * Set the uniform specified by name to the vector (x, y, z).
	 * The program must be in use.
	 */
	void setUniform(const std::string& name, float x, float y, float z);
	/*
	 * Set the uniform specified by name to the vector v.
	 * The program must be in use.
	 */
	void setUniform(const std::string& name, const glm::vec3& v);
	/*
	 * Set the uniform specified by name to the vector v.
	 * The program must be in use.
	 */
	void setUniform(const std::string& name, const glm::vec4& v);
	/*
	 * Set the uniform specified by name to the matrix m.
	 * The program must be in use.
	 */
	void setUniform(const std::string& name, const glm::mat3& m);
	/*
	 * Set the uniform specified by name to the matrix m.
	 * The program must be in use.
	 */
	void setUniform(const std::string& name, const glm::mat4& m);

	/*
	 * Prints a table with all the currently active uniforms used by the program.
	 */
	void printActiveUniforms();

	/*
	 * Prints a table with all the currently active attributes used by the program.
	 */
	void printActiveAttribs();
};

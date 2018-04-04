#include <ShaderProgram.hpp>

#include <GL/glew.h>

ShaderProgram::~ShaderProgram() {
	glDeleteProgram(m_program);
}

void ShaderProgram::use() const {
	glUseProgram(m_program);
}

bool ShaderProgram::isValid() const {
	return m_valid;
}

unsigned int ShaderProgram::getProgram() const {
	return m_program;
}

const std::string& ShaderProgram::getError() const {
	return m_error;
}

bool ShaderProgram::setUniformMatrix2v(const char* name, int count, bool transpose, const float* value) const {
	int uniformLocation = glGetUniformLocation(m_program, name);
	if(uniformLocation == -1)
		return false;

	glUniformMatrix2fv(uniformLocation, count, static_cast<GLboolean>(transpose), value);
	return true;
}

bool ShaderProgram::setUniformMatrix3v(const char* name, int count, bool transpose, const float* value) const {
	int uniformLocation = glGetUniformLocation(m_program, name);
	if(uniformLocation == -1)
		return false;

	glUniformMatrix3fv(uniformLocation, count, static_cast<GLboolean>(transpose), value);
	return true;
}

bool ShaderProgram::setUniformMatrix4v(const char* name, int count, bool transpose, const float* value) const {
	int uniformLocation = glGetUniformLocation(m_program, name);
	if(uniformLocation == -1)
		return false;

	glUniformMatrix4fv(uniformLocation, count, static_cast<GLboolean>(transpose), value);
	return true;
}

bool ShaderProgram::setUniformMatrix2x3v(const char* name, int count, bool transpose, const float* value) const {
	int uniformLocation = glGetUniformLocation(m_program, name);
	if(uniformLocation == -1)
		return false;

	glUniformMatrix2x3fv(uniformLocation, count, static_cast<GLboolean>(transpose), value);
	return true;
}

bool ShaderProgram::setUniformMatrix3x2v(const char* name, int count, bool transpose, const float* value) const {
	int uniformLocation = glGetUniformLocation(m_program, name);
	if(uniformLocation == -1)
		return false;

	glUniformMatrix3x2fv(uniformLocation, count, static_cast<GLboolean>(transpose), value);
	return true;
}

bool ShaderProgram::setUniformMatrix2x4v(const char* name, int count, bool transpose, const float* value) const {
	int uniformLocation = glGetUniformLocation(m_program, name);
	if(uniformLocation == -1)
		return false;

	glUniformMatrix2x4fv(uniformLocation, count, static_cast<GLboolean>(transpose), value);
	return true;
}

bool ShaderProgram::setUniformMatrix4x2v(const char* name, int count, bool transpose, const float* value) const {
	int uniformLocation = glGetUniformLocation(m_program, name);
	if(uniformLocation == -1)
		return false;

	glUniformMatrix4x2fv(uniformLocation, count, static_cast<GLboolean>(transpose), value);
	return true;
}

bool ShaderProgram::setUniformMatrix3x4v(const char* name, int count, bool transpose, const float* value) const {
	int uniformLocation = glGetUniformLocation(m_program, name);
	if(uniformLocation == -1)
		return false;

	glUniformMatrix3x4fv(uniformLocation, count, static_cast<GLboolean>(transpose), value);
	return true;
}

bool ShaderProgram::setUniformMatrix4x3v(const char* name, int count, bool transpose, const float* value) const {
	int uniformLocation = glGetUniformLocation(m_program, name);
	if(uniformLocation == -1)
		return false;

	glUniformMatrix4x3fv(uniformLocation, count, static_cast<GLboolean>(transpose), value);
	return true;
}

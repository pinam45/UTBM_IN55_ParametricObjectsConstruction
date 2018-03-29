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

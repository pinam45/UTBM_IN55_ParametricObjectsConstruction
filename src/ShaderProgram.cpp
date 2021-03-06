/*****************************************************************************************
 *                                                                                       *
 * MIT License                                                                           *
 *                                                                                       *
 * Copyright (c) 2017 Julien Barbier & Jérôme Boulmier & Maxime Pinard                   *
 *                                                                                       *
 * Permission is hereby granted, free of charge, to any person obtaining a copy          *
 * of this software and associated documentation files (the "Software"), to deal         *
 * in the Software without restriction, including without limitation the rights          *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell             *
 * copies of the Software, and to permit persons to whom the Software is                 *
 * furnished to do so, subject to the following conditions:                              *
 *                                                                                       *
 * The above copyright notice and this permission notice shall be included in all        *
 * copies or substantial portions of the Software.                                       *
 *                                                                                       *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR            *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,              *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE           *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER                *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,         *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE         *
 * SOFTWARE.                                                                             *
 *                                                                                       *
 *****************************************************************************************/

#include <utility>

#include <GL/glew.h>

#include "ShaderProgram.hpp"

poc::ShaderProgram::ShaderProgram() noexcept
  : m_valid{false}
  , m_program{}
  , m_error("Shader program uninitialized") {

}

poc::ShaderProgram::~ShaderProgram() {
	if(isValid()) {
		glDeleteProgram(m_program);
	}
}

poc::ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
  : m_valid(std::exchange(other.m_valid, false))
  , m_program(std::exchange(other.m_program, 0))
  , m_error(std::exchange(other.m_error, "Shader program moved")) {

}

bool poc::ShaderProgram::setUniformMatrix2v(const char* name, int count, bool transpose, const float* value) const {
	int uniformLocation = glGetUniformLocation(m_program, name);
	if(uniformLocation == -1)
		return false;

	glUniformMatrix2fv(uniformLocation, count, static_cast<GLboolean>(transpose), value);
	return true;
}

bool poc::ShaderProgram::setUniformMatrix3v(const char* name, int count, bool transpose, const float* value) const {
	int uniformLocation = glGetUniformLocation(m_program, name);
	if(uniformLocation == -1)
		return false;

	glUniformMatrix3fv(uniformLocation, count, static_cast<GLboolean>(transpose), value);
	return true;
}

bool poc::ShaderProgram::setUniformMatrix4v(const char* name, int count, bool transpose, const float* value) const {
	int uniformLocation = glGetUniformLocation(m_program, name);
	if(uniformLocation == -1)
		return false;

	glUniformMatrix4fv(uniformLocation, count, static_cast<GLboolean>(transpose), value);
	return true;
}

bool poc::ShaderProgram::setUniformMatrix2x3v(const char* name, int count, bool transpose, const float* value) const {
	int uniformLocation = glGetUniformLocation(m_program, name);
	if(uniformLocation == -1)
		return false;

	glUniformMatrix2x3fv(uniformLocation, count, static_cast<GLboolean>(transpose), value);
	return true;
}

bool poc::ShaderProgram::setUniformMatrix3x2v(const char* name, int count, bool transpose, const float* value) const {
	int uniformLocation = glGetUniformLocation(m_program, name);
	if(uniformLocation == -1)
		return false;

	glUniformMatrix3x2fv(uniformLocation, count, static_cast<GLboolean>(transpose), value);
	return true;
}

bool poc::ShaderProgram::setUniformMatrix2x4v(const char* name, int count, bool transpose, const float* value) const {
	int uniformLocation = glGetUniformLocation(m_program, name);
	if(uniformLocation == -1)
		return false;

	glUniformMatrix2x4fv(uniformLocation, count, static_cast<GLboolean>(transpose), value);
	return true;
}

bool poc::ShaderProgram::setUniformMatrix4x2v(const char* name, int count, bool transpose, const float* value) const {
	int uniformLocation = glGetUniformLocation(m_program, name);
	if(uniformLocation == -1)
		return false;

	glUniformMatrix4x2fv(uniformLocation, count, static_cast<GLboolean>(transpose), value);
	return true;
}

bool poc::ShaderProgram::setUniformMatrix3x4v(const char* name, int count, bool transpose, const float* value) const {
	int uniformLocation = glGetUniformLocation(m_program, name);
	if(uniformLocation == -1)
		return false;

	glUniformMatrix3x4fv(uniformLocation, count, static_cast<GLboolean>(transpose), value);
	return true;
}

bool poc::ShaderProgram::setUniformMatrix4x3v(const char* name, int count, bool transpose, const float* value) const {
	int uniformLocation = glGetUniformLocation(m_program, name);
	if(uniformLocation == -1)
		return false;

	glUniformMatrix4x3fv(uniformLocation, count, static_cast<GLboolean>(transpose), value);
	return true;
}

void poc::ShaderProgram::use() const {
	glUseProgram(m_program);
}

bool poc::ShaderProgram::isValid() const {
	return m_valid;
}

unsigned int poc::ShaderProgram::getProgram() const {
	return m_program;
}

const std::string& poc::ShaderProgram::getError() const {
	return m_error;
}

int poc::ShaderProgram::getAttributeLocation(const char* name) const {
	return glGetAttribLocation(m_program, name);
}

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

#include <fstream>
#include <utility>

#include <GL/glew.h>

#include "Shader.hpp"

static constexpr unsigned int POSSIBLE_SHADER_TYPES[] = {
  GL_COMPUTE_SHADER,
  GL_VERTEX_SHADER,
  GL_TESS_CONTROL_SHADER,
  GL_TESS_EVALUATION_SHADER,
  GL_GEOMETRY_SHADER,
  GL_FRAGMENT_SHADER
};

poc::Shader::Shader(unsigned int type)
  : m_valid(true)
  , m_type(type)
  , m_shader()
  , m_error() {

	for(unsigned int shader_type : POSSIBLE_SHADER_TYPES) {
		if(m_type == shader_type) {
			return;
		}
	}
	m_error = "Invalid shader type: " + std::to_string(type);
	m_valid = false;
}

poc::Shader::Shader(Shader&& other) noexcept
  : m_valid(std::exchange(other.m_valid, false))
  , m_type(std::exchange(other.m_type, 0))
  , m_shader(std::exchange(other.m_shader, 0))
  , m_error(std::exchange(other.m_error, "Shader was moved")) {

}

void poc::Shader::loadShader(const char* text) {
	m_shader = glCreateShader(m_type);

	glShaderSource(m_shader, 1, &text, nullptr);
	glCompileShader(m_shader);

	int success;
	glGetShaderiv(m_shader, GL_COMPILE_STATUS, &success);
	if(!success) {
		char infoLog[512];
		glGetShaderInfoLog(m_shader, 512, nullptr, infoLog);
		m_valid = false;
		m_error = "Shader compilation failed:\n";
		m_error += infoLog;
	}
}

poc::Shader poc::Shader::fromString(unsigned int type, const std::string& text) {
	return fromString(type, text.c_str());
}

poc::Shader poc::Shader::fromString(unsigned int type, const char* text) {
	Shader shader(type);
	if(!shader.isValid()) {
		return shader;
	}

	if(!text) {
		shader.m_valid = false;
		shader.m_error = "nullptr passed as text";
	}

	shader.loadShader(text);
	return shader;
}

poc::Shader poc::Shader::fromFile(unsigned int type, const std::string& path) {
	return fromFile(type, path.c_str());
}

poc::Shader poc::Shader::fromFile(unsigned int type, const char* path) {
	Shader shader(type);
	if(!shader.isValid()) {
		return shader;
	}

	if(!path) {
		shader.m_valid = false;
		shader.m_error = "nullptr passed as path";
	}

	std::ifstream file(path);
	if(!file) {
		shader.m_error = "File " + std::string(path) + " opening failed";
		shader.m_valid = false;
		return shader;
	}

	file.seekg(0, std::ios::end);
	std::string shader_text;
	shader_text.reserve(static_cast<size_t>(file.tellg()));
	file.seekg(0, std::ios::beg);
	shader_text.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
	file.close();

	shader.loadShader(shader_text.c_str());
	return shader;
}

poc::Shader::~Shader() {
	if(isValid()) {
		glDeleteShader(m_shader);
	}
}

bool poc::Shader::isValid() const {
	return m_valid;
}

unsigned int poc::Shader::getType() const {
	return m_type;
}

unsigned int poc::Shader::getShader() const {
	return m_shader;
}

const std::string& poc::Shader::getError() const {
	return m_error;
}

#include <Shader.hpp>

#include <GL/glew.h>
#include <fstream>

static constexpr unsigned int POSSIBLE_SHADER_TYPES[] = {
  GL_COMPUTE_SHADER,
  GL_VERTEX_SHADER,
  GL_TESS_CONTROL_SHADER,
  GL_TESS_EVALUATION_SHADER,
  GL_GEOMETRY_SHADER,
  GL_FRAGMENT_SHADER
};

Shader::Shader(unsigned int type)
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

void Shader::loadShader(const char* text) {
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

Shader Shader::fromString(unsigned int type, const std::string& text) {
	return fromString(type, text.c_str());
}

Shader Shader::fromString(unsigned int type, const char* text) {
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

Shader Shader::fromFile(unsigned int type, const std::string& path) {
	return fromFile(type, path.c_str());
}

Shader Shader::fromFile(unsigned int type, const char* path) {
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

Shader::~Shader() {
	glDeleteShader(m_shader);
}

bool Shader::isValid() const {
	return m_valid;
}

unsigned int Shader::getType() const {
	return m_type;
}

unsigned int Shader::getShader() const {
	return m_shader;
}

const std::string& Shader::getError() const {
	return m_error;
}
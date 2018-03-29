#ifndef PARAMETRICOBJECTSCONSTRUCTION_SHADERPROGRAM_HPP
#define PARAMETRICOBJECTSCONSTRUCTION_SHADERPROGRAM_HPP


#include <GL/glew.h>
#include <Shader.hpp>

class ShaderProgram {

public:
	template<typename... T>
	explicit ShaderProgram(const T&... shaders);

	virtual ~ShaderProgram();

	// Program need to be in use
	template<typename T>
	void setUniform1(const char* name, T v0) const;

	// Program need to be in use
	template<typename T>
	void setUniform2(const char* name, T v0, T v1) const;

	// Program need to be in use
	template<typename T>
	void setUniform3(const char* name, T v0, T v1, T v2) const;

	// Program need to be in use
	template<typename T>
	void setUniform4(const char* name, T v0, T v1, T v2, T v3) const;

	void use() const;

	bool isValid() const;

	unsigned int getProgram() const;

	const std::string& getError() const;

private:
	template<typename T>
	void setUniform1(int location, T v0) const;

	template<typename T>
	void setUniform2(int location, T v0, T v1) const;

	template<typename T>
	void setUniform3(int location, T v0, T v1, T v2) const;

	template<typename T>
	void setUniform4(int location, T v0, T v1, T v2, T v3) const;


	bool m_valid;
	unsigned int m_program;

	std::string m_error;
};

template<typename... T>
ShaderProgram::ShaderProgram(const T&... shaders)
  : m_valid(true)
    , m_program()
    , m_error(){

	static_assert((std::is_same<T, Shader>::value && ...));

	m_program = glCreateProgram();

	(glAttachShader(m_program, shaders.getShader()), ...);
	glLinkProgram(m_program);

	int success;
	glGetProgramiv(m_program, GL_LINK_STATUS, &success);
	if(!success) {
		char infoLog[512];
		glGetShaderInfoLog(m_program, 512, nullptr, infoLog);
		m_valid = false;
		m_error = "Shader program link failed:\n";
		m_error += infoLog;
	}
}

template<typename T>
void ShaderProgram::setUniform1(const char* name, T v0) const {
	static_assert(
	  std::is_same<T, float>::value
	  || std::is_same<T, int>::value
	  || std::is_same<T, unsigned int>::value
	);

	int uniformLocation = glGetUniformLocation(m_program, name);
	if(uniformLocation == -1)
		return;

	setUniform1(uniformLocation, v0);
}

template<typename T>
void ShaderProgram::setUniform2(const char* name, T v0, T v1) const {
	static_assert(
	  std::is_same<T, float>::value
	  || std::is_same<T, int>::value
	  || std::is_same<T, unsigned int>::value
	);

	int uniformLocation = glGetUniformLocation(m_program, name);
	if(uniformLocation == -1)
		return;

	setUniform1(uniformLocation, v0, v1);
}

template<typename T>
void ShaderProgram::setUniform3(const char* name, T v0, T v1, T v2) const {
	static_assert(
	  std::is_same<T, float>::value
	  || std::is_same<T, int>::value
	  || std::is_same<T, unsigned int>::value
	);

	int uniformLocation = glGetUniformLocation(m_program, name);
	if(uniformLocation == -1)
		return;

	setUniform1(uniformLocation, v0, v1, v2);
}

template<typename T>
void ShaderProgram::setUniform4(const char* name, T v0, T v1, T v2, T v3) const {
	static_assert(
	  std::is_same<T, float>::value
	  || std::is_same<T, int>::value
	  || std::is_same<T, unsigned int>::value
	);

	int uniformLocation = glGetUniformLocation(m_program, name);
	if(uniformLocation == -1)
		return;

	setUniform1(uniformLocation, v0, v1, v2, v3);
}

template<>
inline void ShaderProgram::setUniform1<float>(int location, float v0) const {
	glUniform1f(location, v0);
}

template<>
inline void ShaderProgram::setUniform1<int>(int location, int v0) const {
	glUniform1i(location, v0);
}

template<>
inline void ShaderProgram::setUniform1<unsigned int>(int location, unsigned int v0) const {
	glUniform1ui(location, v0);
}

template<>
inline void ShaderProgram::setUniform2<float>(int location, float v0, float v1) const {
	glUniform2f(location, v0, v1);
}

template<>
inline void ShaderProgram::setUniform2<int>(int location, int v0, int v1) const {
	glUniform2i(location, v0, v1);
}

template<>
inline void ShaderProgram::setUniform2<unsigned int>(int location, unsigned int v0, unsigned int v1) const {
	glUniform2ui(location, v0, v1);
}

template<>
inline void ShaderProgram::setUniform3<float>(int location, float v0, float v1, float v2) const {
	glUniform3f(location, v0, v1, v2);
}

template<>
inline void ShaderProgram::setUniform3<int>(int location, int v0, int v1, int v2) const {
	glUniform3i(location, v0, v1, v2);
}

template<>
inline void ShaderProgram::setUniform3<unsigned int>(int location, unsigned int v0, unsigned int v1, unsigned int v2) const {
	glUniform3ui(location, v0, v1, v2);
}

template<>
inline void ShaderProgram::setUniform4<float>(int location, float v0, float v1, float v2, float v3) const {
	glUniform4f(location, v0, v1, v2, v3);
}

template<>
inline void ShaderProgram::setUniform4<int>(int location, int v0, int v1, int v2, int v3) const {
	glUniform4i(location, v0, v1, v2, v3);
}

template<>
inline void ShaderProgram::setUniform4<unsigned int>(int location, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3) const {
	glUniform4ui(location, v0, v1, v2, v3);
}


#endif //PARAMETRICOBJECTSCONSTRUCTION_SHADERPROGRAM_HPP

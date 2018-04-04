#ifndef PARAMETRICOBJECTSCONSTRUCTION_SHADERPROGRAM_HPP
#define PARAMETRICOBJECTSCONSTRUCTION_SHADERPROGRAM_HPP


#include <GL/glew.h>
#include <Shader.hpp>

template<typename>
struct dependent_false: public std::false_type{ };

class ShaderProgram {

public:
	template<typename... T>
	explicit ShaderProgram(const T&... shaders);

	virtual ~ShaderProgram();

	// Program need to be in use to set uniforms
	template<typename T>
	void setUniform1(const char* name, T v0) const;

	template<typename T>
	void setUniform2(const char* name, T v0, T v1) const;

	template<typename T>
	void setUniform3(const char* name, T v0, T v1, T v2) const;

	template<typename T>
	void setUniform4(const char* name, T v0, T v1, T v2, T v3) const;

	void use() const;

	bool isValid() const;

	unsigned int getProgram() const;

	const std::string& getError() const;

private:

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
	int uniformLocation = glGetUniformLocation(m_program, name);
	if(uniformLocation == -1)
		return;

	if constexpr(std::is_same<T, float>::value){
		glUniform1f(uniformLocation, v0);
	}
	else if constexpr(std::is_same<T, int>::value){
		glUniform1i(uniformLocation, v0);
	}
	else if constexpr(std::is_same<T, unsigned int>::value){
		glUniform1ui(uniformLocation, v0);
	}
	else{
		static_assert(dependent_false<T>::value, "Unsupported type");
	}
}

template<typename T>
void ShaderProgram::setUniform2(const char* name, T v0, T v1) const {
	int uniformLocation = glGetUniformLocation(m_program, name);
	if(uniformLocation == -1)
		return;

	if constexpr(std::is_same<T, float>::value){
		glUniform2f(uniformLocation, v0, v1);
	}
	else if constexpr(std::is_same<T, int>::value){
		glUniform2i(uniformLocation, v0, v1);
	}
	else if constexpr(std::is_same<T, unsigned int>::value){
		glUniform2ui(uniformLocation, v0, v1);
	}
	else{
		static_assert(dependent_false<T>::value, "Unsupported type");
	}
}

template<typename T>
void ShaderProgram::setUniform3(const char* name, T v0, T v1, T v2) const {
	int uniformLocation = glGetUniformLocation(m_program, name);
	if(uniformLocation == -1)
		return;

	if constexpr(std::is_same<T, float>::value){
		glUniform3f(uniformLocation, v0, v1, v2);
	}
	else if constexpr(std::is_same<T, int>::value){
		glUniform3i(uniformLocation, v0, v1, v2);
	}
	else if constexpr(std::is_same<T, unsigned int>::value){
		glUniform3ui(uniformLocation, v0, v1, v2);
	}
	else{
		static_assert(dependent_false<T>::value, "Unsupported type");
	}
}

template<typename T>
void ShaderProgram::setUniform4(const char* name, T v0, T v1, T v2, T v3) const {
	int uniformLocation = glGetUniformLocation(m_program, name);
	if(uniformLocation == -1)
		return;

	if constexpr(std::is_same<T, float>::value){
		glUniform4f(uniformLocation, v0, v1, v2, v3);
	}
	else if constexpr(std::is_same<T, int>::value){
		glUniform4i(uniformLocation, v0, v1, v2, v3);
	}
	else if constexpr(std::is_same<T, unsigned int>::value){
		glUniform4ui(uniformLocation, v0, v1, v2, v3);
	}
	else{
		static_assert(dependent_false<T>::value, "Unsupported type");
	}
}


#endif //PARAMETRICOBJECTSCONSTRUCTION_SHADERPROGRAM_HPP

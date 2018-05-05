#ifndef PARAMETRICOBJECTSCONSTRUCTION_SHADERPROGRAM_HPP
#define PARAMETRICOBJECTSCONSTRUCTION_SHADERPROGRAM_HPP

#include <GL/glew.h>

#include "Shader.hpp"

namespace poc {
	template <typename>
	struct dependent_false : public std::false_type {
	};

	class ShaderProgram {
	public:
		ShaderProgram() noexcept;

		template <typename... Shaders>
		explicit ShaderProgram(Shaders&& ... shaders);

		ShaderProgram(const ShaderProgram&) = delete;

		ShaderProgram(ShaderProgram&&) noexcept;

		virtual ~ShaderProgram();

		template <typename... Shaders>
		void setShaders(Shaders&& ... shaders);

		// Program need to be in use to set uniforms
		template <typename T>
		bool setUniform1(const char* name, T v0) const;

		template <typename T>
		bool setUniform2(const char* name, T v0, T v1) const;

		template <typename T>
		bool setUniform3(const char* name, T v0, T v1, T v2) const;

		template <typename T>
		bool setUniform4(const char* name, T v0, T v1, T v2, T v3) const;

		template <typename T>
		bool setUniform1v(const char* name, int count, const T* value) const;

		template <typename T>
		bool setUniform2v(const char* name, int count, const T* value) const;

		template <typename T>
		bool setUniform3v(const char* name, int count, const T* value) const;

		template <typename T>
		bool setUniform4v(const char* name, int count, const T* value) const;

		bool setUniformMatrix2v(const char* name, int count, bool transpose, const float* value) const;

		bool setUniformMatrix3v(const char* name, int count, bool transpose, const float* value) const;

		bool setUniformMatrix4v(const char* name, int count, bool transpose, const float* value) const;

		bool setUniformMatrix2x3v(const char* name, int count, bool transpose, const float* value) const;

		bool setUniformMatrix3x2v(const char* name, int count, bool transpose, const float* value) const;

		bool setUniformMatrix2x4v(const char* name, int count, bool transpose, const float* value) const;

		bool setUniformMatrix4x2v(const char* name, int count, bool transpose, const float* value) const;

		bool setUniformMatrix3x4v(const char* name, int count, bool transpose, const float* value) const;

		bool setUniformMatrix4x3v(const char* name, int count, bool transpose, const float* value) const;

		int getAttributeLocation(const char* name) const;

		void use() const;

		bool isValid() const;

		unsigned int getProgram() const;

		const std::string& getError() const;

	private:

		bool m_valid;
		unsigned int m_program;

		std::string m_error;
	};
}

template<typename... Shaders>
poc::ShaderProgram::ShaderProgram(Shaders&&... shaders)
        : m_valid(true)
        , m_program()
        , m_error() {

	setShaders(std::forward<Shaders>(shaders)...);
}


template <typename... Shaders>
void poc::ShaderProgram::setShaders(Shaders&& ... shaders) {
	static_assert((std::is_same_v<std::decay_t<Shaders>, Shader> && ...));

	if (m_program != 0) {
		glDeleteProgram(m_program);
	}

	m_program = glCreateProgram();
	if (m_program == 0) {
		m_valid = false;
		m_error = "Shader program creation failed:\n";

		char infoLog[512];
		glGetShaderInfoLog(m_program, 512, nullptr, infoLog);
		m_error += infoLog;
	}

	(glAttachShader(m_program, shaders.getShader()), ...);
	glLinkProgram(m_program);

	int success;
	glGetProgramiv(m_program, GL_LINK_STATUS, &success);

	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(m_program, 512, nullptr, infoLog);
		m_valid = false;
		m_error = "Shader program link failed:\n";
		m_error += infoLog;
	}
}

template<typename T>
bool poc::ShaderProgram::setUniform1(const char* name, T v0) const {
	int uniformLocation = glGetUniformLocation(m_program, name);
	if(uniformLocation == -1)
		return false;

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
	return true;
}

template<typename T>
bool poc::ShaderProgram::setUniform2(const char* name, T v0, T v1) const {
	int uniformLocation = glGetUniformLocation(m_program, name);
	if(uniformLocation == -1)
		return false;

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
	return true;
}

template<typename T>
bool poc::ShaderProgram::setUniform3(const char* name, T v0, T v1, T v2) const {
	int uniformLocation = glGetUniformLocation(m_program, name);
	if(uniformLocation == -1)
		return false;

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
	return true;
}

template<typename T>
bool poc::ShaderProgram::setUniform4(const char* name, T v0, T v1, T v2, T v3) const {
	int uniformLocation = glGetUniformLocation(m_program, name);
	if(uniformLocation == -1)
		return false;

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
	return true;
}

template<typename T>
bool poc::ShaderProgram::setUniform1v(const char* name, int count, const T* value) const {
	int uniformLocation = glGetUniformLocation(m_program, name);
	if(uniformLocation == -1)
		return false;

	if constexpr(std::is_same<T, float>::value){
		glUniform1fv(uniformLocation, count, value);
	}
	else if constexpr(std::is_same<T, int>::value){
		glUniform1iv(uniformLocation, count, value);
	}
	else if constexpr(std::is_same<T, unsigned int>::value){
		glUniform1uiv(uniformLocation, count, value);
	}
	else{
		static_assert(dependent_false<T>::value, "Unsupported type");
	}
	return true;
}

template<typename T>
bool poc::ShaderProgram::setUniform2v(const char* name, int count, const T* value) const {
	int uniformLocation = glGetUniformLocation(m_program, name);
	if(uniformLocation == -1)
		return false;

	if constexpr(std::is_same<T, float>::value){
		glUniform2fv(uniformLocation, count, value);
	}
	else if constexpr(std::is_same<T, int>::value){
		glUniform2iv(uniformLocation, count, value);
	}
	else if constexpr(std::is_same<T, unsigned int>::value){
		glUniform2uiv(uniformLocation, count, value);
	}
	else{
		static_assert(dependent_false<T>::value, "Unsupported type");
	}
	return true;
}

template<typename T>
bool poc::ShaderProgram::setUniform3v(const char* name, int count, const T* value) const {
	int uniformLocation = glGetUniformLocation(m_program, name);
	if(uniformLocation == -1)
		return false;

	if constexpr(std::is_same<T, float>::value){
		glUniform3fv(uniformLocation, count, value);
	}
	else if constexpr(std::is_same<T, int>::value){
		glUniform3iv(uniformLocation, count, value);
	}
	else if constexpr(std::is_same<T, unsigned int>::value){
		glUniform3uiv(uniformLocation, count, value);
	}
	else{
		static_assert(dependent_false<T>::value, "Unsupported type");
	}
	return true;
}

template<typename T>
bool poc::ShaderProgram::setUniform4v(const char* name, int count, const T* value) const {
	int uniformLocation = glGetUniformLocation(m_program, name);
	if(uniformLocation == -1)
		return false;

	if constexpr(std::is_same<T, float>::value){
		glUniform4fv(uniformLocation, count, value);
	}
	else if constexpr(std::is_same<T, int>::value){
		glUniform4iv(uniformLocation, count, value);
	}
	else if constexpr(std::is_same<T, unsigned int>::value){
		glUniform4uiv(uniformLocation, count, value);
	}
	else{
		static_assert(dependent_false<T>::value, "Unsupported type");
	}
	return true;
}

#endif //PARAMETRICOBJECTSCONSTRUCTION_SHADERPROGRAM_HPP

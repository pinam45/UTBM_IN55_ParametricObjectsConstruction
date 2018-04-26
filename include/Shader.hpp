#ifndef PARAMETRICOBJECTSCONSTRUCTION_SHADER_HPP
#define PARAMETRICOBJECTSCONSTRUCTION_SHADER_HPP


#include <string>

namespace poc {
	class Shader {
	public:
		static Shader fromString(unsigned int type, const std::string& text);

		static Shader fromString(unsigned int type, const char* text);

		static Shader fromFile(unsigned int type, const std::string& path);

		static Shader fromFile(unsigned int type, const char* path);

		Shader(const Shader&) = delete;

		Shader(Shader&& other) noexcept;

		virtual ~Shader();

		bool isValid() const;

		unsigned int getType() const;

		unsigned int getShader() const;

		const std::string& getError() const;

	private:
		explicit Shader(unsigned int type);

		void loadShader(const char* text);

		bool m_valid;
		unsigned int m_type;
		unsigned int m_shader;

		std::string m_error;
	};
}

#endif //PARAMETRICOBJECTSCONSTRUCTION_SHADER_HPP

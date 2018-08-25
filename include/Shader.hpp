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

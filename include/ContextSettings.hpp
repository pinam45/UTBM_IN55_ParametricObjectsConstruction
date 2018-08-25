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

#ifndef PARAMETRICOBJECTSCONSTRUCTION_CONTEXTSETTINGS_HPP
#define PARAMETRICOBJECTSCONSTRUCTION_CONTEXTSETTINGS_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace poc {
	enum class Mode : int {
		Any = GLFW_OPENGL_ANY_PROFILE,
		Core = GLFW_OPENGL_CORE_PROFILE,
		Compat = GLFW_OPENGL_COMPAT_PROFILE
	};

	struct ContextSettings {
		explicit ContextSettings(int depthBits = 0, int stencilBits = 0,
		                int antialiasingLevel = 0,
		                int major = 3, int minor = 3, Mode mode = Mode::Core) noexcept;


		int depthBits;
		int stencilBits;
		int antialiasingLevel;
		int major;
		int minor;
		Mode mode;
	};
}


#endif //PARAMETRICOBJECTSCONSTRUCTION_CONTEXTSETTINGS_HPP

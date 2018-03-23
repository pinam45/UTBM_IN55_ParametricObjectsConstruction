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

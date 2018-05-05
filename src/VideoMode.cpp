#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "VideoMode.hpp"

poc::VideoMode::VideoMode(int width_, int height_,
                          int redBits_, int greenBits_,
                          int blueBits_, int refreshRate_)
	: width(width_)
	, height(height_)
	, redBits(redBits_)
	, greenBits(greenBits_)
	, blueBits(blueBits_)
	, refreshRate(refreshRate_)
	{

}

poc::VideoMode poc::VideoMode::getPrimaryMonitorVideoMode() {
	GLFWmonitor* pmonitor = glfwGetPrimaryMonitor();

	if (pmonitor == nullptr) {
		return poc::VideoMode(0, 0, 0, 0, 0);
	}

	const GLFWvidmode* vmode = glfwGetVideoMode(pmonitor);

	return poc::VideoMode(vmode->width, vmode->height, vmode->redBits, vmode->greenBits, vmode->blueBits, vmode->refreshRate);
}

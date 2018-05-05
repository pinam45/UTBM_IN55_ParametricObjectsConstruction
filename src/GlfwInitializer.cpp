#include <stdexcept>

#include "GlfwInitializer.hpp"
#include "callbacks.hpp"

poc::GlfwInitializer::GlfwInitializer() {
	if (!glfwInit()) {
		throw std::runtime_error("GLFW error: Initialization failed");
	}

	glfwSetErrorCallback(error_callback);
}

poc::GlfwInitializer::~GlfwInitializer() {
	glfwTerminate();
}

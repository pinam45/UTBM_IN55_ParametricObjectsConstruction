#include <iostream>

#include "callbacks.hpp"

void error_callback(int error, const char* description) {
	std::cerr << "GLFW error " << error << ": " << description << std::endl;
}

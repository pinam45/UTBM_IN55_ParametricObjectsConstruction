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

#include <sstream>
#include <stdexcept>

#include <imgui.h>

#include "Window.hpp"
#include "Event.hpp"

#include "implementation/ImguiImpl.hpp"

namespace {
	ImGuiContext* s_imguiContext;
}

poc::Window::Window(const poc::VideoMode& videoMode, std::string_view title, FullscreenMode fullscreenMode,
                    const poc::ContextSettings& contextSettings)
  : m_window{nullptr}
    , m_events{32} {

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, contextSettings.major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, contextSettings.minor);

	glfwWindowHint(GLFW_RED_BITS, videoMode.redBits);
	glfwWindowHint(GLFW_GREEN_BITS, videoMode.greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, videoMode.blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, videoMode.refreshRate);

	switch(fullscreenMode) {
		case FullscreenMode::NoFullScreen:
			m_window = glfwCreateWindow(videoMode.width, videoMode.height, title.data(), nullptr, nullptr);
			break;
		case FullscreenMode::WindowedFullScreen:
			m_window = glfwCreateWindow(videoMode.width, videoMode.height, title.data(), glfwGetPrimaryMonitor(), nullptr);
			break;
	}

	if(m_window == nullptr) {
		glfwTerminate();
		throw std::runtime_error("GLFW error: Failed to create window.");
	}

	m_isOpen = true;

	glfwMakeContextCurrent(m_window);
	glfwSetWindowUserPointer(m_window, this);

	auto keyCallback = [](GLFWwindow* w, int key, int scancode, int action, int mods) {
		if(!ImGui::GetIO().WantCaptureKeyboard) {
			static_cast<poc::Window*>(glfwGetWindowUserPointer(w))->createKeyEvent(key, scancode, action, mods);
		} else {
			poc::ImguiImpl::onKey(key, scancode, action, mods);
		}
	};

	auto textCallback = [](GLFWwindow* w, unsigned int character) {
		if(!ImGui::GetIO().WantTextInput) {
			static_cast<poc::Window*>(glfwGetWindowUserPointer(w))->createCharEvent(character);
		} else {
			poc::ImguiImpl::onChar(character);
		}
	};

	auto mouseCallback = [](GLFWwindow* w, int button, int action, int mods) {
		if(!ImGui::GetIO().WantCaptureMouse) {
			static_cast<poc::Window*>(glfwGetWindowUserPointer(w))->createMouseEvent(button, action, mods);
		} else {
			poc::ImguiImpl::onMouseButton(button, action, mods);
		}
	};

	auto cursorPosCallback = [](GLFWwindow* w, double xPos, double yPos) {
		if(!ImGui::GetIO().WantCaptureMouse) {
			static_cast<poc::Window*>(glfwGetWindowUserPointer(w))->createMouseEvent(xPos, yPos);
		}
	};

	auto mouseScrollCallback = [](GLFWwindow* w, double xPos, double yPos) {
		if(!ImGui::GetIO().WantCaptureMouse) {
			static_cast<poc::Window*>(glfwGetWindowUserPointer(w))->createMouseScrollEvent(xPos, yPos);
		} else {
			poc::ImguiImpl::onScroll(xPos, yPos);
		}
	};

	auto windowCloseCallback = [](GLFWwindow* w) {
		static_cast<poc::Window*>(glfwGetWindowUserPointer(w))->createCloseEvent();
	};

	auto windowResizeCallback = [](GLFWwindow* w, int width, int height) {
		static_cast<poc::Window*>(glfwGetWindowUserPointer(w))->createResizeEvent(width, height);
		glViewport(0, 0, width, height);
	};

	auto windowFocusCallback = [](GLFWwindow* w, int focused) {
		static_cast<poc::Window*>(glfwGetWindowUserPointer(w))->createFocusEvent(focused);
	};

	glfwSetKeyCallback(m_window, keyCallback);
	glfwSetCharCallback(m_window, textCallback);
	glfwSetMouseButtonCallback(m_window, mouseCallback);
	glfwSetCursorPosCallback(m_window, cursorPosCallback);
	glfwSetScrollCallback(m_window, mouseScrollCallback);
	glfwSetWindowCloseCallback(m_window, windowCloseCallback);
	glfwSetFramebufferSizeCallback(m_window, windowResizeCallback);
	glfwSetWindowFocusCallback(m_window, windowFocusCallback);

	GLenum err = glewInit();
	if(err != GLEW_OK) {
		glfwTerminate();
		std::stringstream sstream;
		sstream << "GLEW Error: " << glewGetErrorString(err) << '\n';

		throw std::runtime_error(sstream.str());
	}

	ImGui::CreateContext();
	s_imguiContext = ImGui::GetCurrentContext();
	ImguiImpl::init(m_window);
	ImguiImpl::newFrame();
}

poc::Window::~Window() {
	if(m_window) {
		glfwDestroyWindow(m_window);
		ImguiImpl::shutdown();
	}
}

bool poc::Window::isOpen() const {
	return m_isOpen;
}

void poc::Window::close() {
	if(isOpen()) {
		m_isOpen = false;
		glfwSetWindowShouldClose(m_window, GLFW_TRUE);
		glfwDestroyWindow(m_window);
		m_window = nullptr;

		m_events.clear();
	}
}

bool poc::Window::pollEvent(poc::Event& event) {
	if(m_events.empty()) {
		return false;
	}

	event = std::move(m_events.front());
	m_events.pop_front();

	return true;
}

void poc::Window::display() {
	if(!isOpen()) {
		return;
	}

	ImguiImpl::render();

	glfwPollEvents();
	m_isOpen = !static_cast<bool>(glfwWindowShouldClose(m_window));

	glfwSwapBuffers(m_window);

	if(m_isOpen) {
		ImguiImpl::newFrame();
	}
}

int poc::Window::getWidth() const {
	int width, height;
	glfwGetWindowSize(m_window, &width, &height);
	return width;
}

int poc::Window::getHeigth() const {
	int width, height;
	glfwGetWindowSize(m_window, &width, &height);
	return height;
}

void poc::Window::setInputMode(int mode, int value) {
	glfwSetInputMode(m_window, mode, value);
}

bool poc::Window::isPressed(Keyboard::Key key) const {
	return glfwGetKey(m_window, static_cast<int>(key)) == GLFW_PRESS;
}

bool poc::Window::isPressed(Mouse::Button button) const {
	return glfwGetMouseButton(m_window, static_cast<int>(button)) == GLFW_PRESS;
}

void poc::Window::createKeyEvent(int key, int, int action, int mods) {
	const EventType type = [&action] {
		switch(action) {
			case GLFW_PRESS:
				return EventType::KeyPressed;
			case GLFW_RELEASE:
				return EventType::KeyReleased;
			case GLFW_REPEAT:
				return EventType::KeyRepeat;
			default:
				return EventType::Unknown;
		}
	}();

	m_events.emplace_back(type, std::in_place_type_t<Event::KeyEvent>{}, static_cast<Keyboard::Key>(key),
	                      static_cast<bool>(mods & GLFW_MOD_SHIFT),
	                      static_cast<bool>(mods & GLFW_MOD_CONTROL),
	                      static_cast<bool>(mods & GLFW_MOD_ALT),
	                      static_cast<bool>(mods & GLFW_MOD_SUPER));
}

void poc::Window::createCharEvent(unsigned int character) {
	m_events.emplace_back(EventType::TextEntered, std::in_place_type_t<Event::TextEvent>{}, character);
}

void poc::Window::createMouseEvent(int button, int action, int) {
	const EventType type = [&action] {
		switch(action) {
			case GLFW_PRESS:
				return EventType::MouseButtonPressed;
			case GLFW_RELEASE:
				return EventType::MouseButtonReleased;
			default:
				return EventType::Unknown;
		}
	}();

	double xPos = -1.;
	double yPos = -1.;
	glfwGetCursorPos(m_window, &xPos, &yPos);

	m_events.emplace_back(type, std::in_place_type_t<Event::MouseButtonEvent>{}, static_cast<Mouse::Button>(button),
	                      static_cast<int>(xPos),
	                      static_cast<int>(yPos));
}

void poc::Window::createCloseEvent() {
	m_events.emplace_back(EventType::Closed);
}

void poc::Window::createMouseEvent(double xPos, double yPos) {
	m_events.emplace_back(EventType::MouseMoved, std::in_place_type_t<Event::MouseMoveEvent>{},
	                      static_cast<int>(xPos),
	                      static_cast<int>(yPos));
}

void poc::Window::createMouseScrollEvent(double xOffset, double yOffset) {
	m_events.emplace_back(EventType::MouseScrollEvent, std::in_place_type_t<Event::MouseScrollEvent>{},
	                      xOffset,
	                      yOffset);
}

void poc::Window::createResizeEvent(int width, int height) {
	m_events.emplace_back(EventType::Resized, std::in_place_type_t<Event::ResizeEvent>{}, width, height);
}

void poc::Window::createFocusEvent(int focused) {
	m_events.emplace_back(focused ? EventType::FocusGain : EventType::FocusLost);
}

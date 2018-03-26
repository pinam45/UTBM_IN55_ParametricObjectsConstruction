#include <stdexcept>

#include <Window.hpp>
#include <Event.hpp>

#include <imgui.h>
#include <sstream>
#include "implementation/ImguiImpl.hpp"

poc::Window::Window(const poc::VideoMode& videoMode, std::string_view title, FullscreenMode fullscreenMode,
                    const poc::ContextSettings& contextSettings)
	: m_window{nullptr}
	, m_events{} {

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, contextSettings.major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, contextSettings.minor);

	glfwWindowHint(GLFW_RED_BITS, videoMode.redBits);
	glfwWindowHint(GLFW_GREEN_BITS, videoMode.greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, videoMode.blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, videoMode.refreshRate);

	switch (fullscreenMode) {
		case FullscreenMode::NoFullScreen:
			m_window = glfwCreateWindow(videoMode.width, videoMode.height, title.data(), nullptr, nullptr);
			break;
		case FullscreenMode::WindowedFullScreen:
			m_window = glfwCreateWindow(videoMode.width, videoMode.height, title.data(), glfwGetPrimaryMonitor(), nullptr);
			break;
	}

	if (m_window == nullptr) {
		glfwTerminate();
		throw std::runtime_error("GLFW error: Failed to create window.");
	}

	m_isOpen = true;

	glfwMakeContextCurrent(m_window);
	glfwSetWindowUserPointer(m_window, this);

	auto keyCallback = [](GLFWwindow* w, int key, int scancode, int action, int mods) {
		poc::ImguiImpl::onKey(key, scancode, action, mods);
		static_cast<poc::Window*>(glfwGetWindowUserPointer(w))->createKeyEvent(key, scancode, action, mods);
	};

	auto mouseCallback = [](GLFWwindow* w, int button, int action, int mods) {
		poc::ImguiImpl::onMouseButton(button, action, mods);
		static_cast<poc::Window*>(glfwGetWindowUserPointer(w))->createMouseEvent(button, action, mods);
	};

	auto cursorPosCallback = [](GLFWwindow* w, double xPos, double yPos) {
		static_cast<poc::Window*>(glfwGetWindowUserPointer(w))->createMouseEvent(xPos, yPos);
	};

	auto mouseScrollCallback = [](GLFWwindow* w, double xPos, double yPos) {
		poc::ImguiImpl::onScroll(xPos, yPos);
		static_cast<poc::Window*>(glfwGetWindowUserPointer(w))->createMouseScrollEvent(xPos, yPos);
	};

	auto windowCloseCallback = [](GLFWwindow* w) {
		static_cast<poc::Window*>(glfwGetWindowUserPointer(w))->createCloseEvent();
	};

	auto windowResizeCallback = [](GLFWwindow* w, int width, int height) {
		static_cast<poc::Window*>(glfwGetWindowUserPointer(w))->createResizeEvent(width, height);
	};

	auto windowFocusCallback = [](GLFWwindow* w, int focused) {
		static_cast<poc::Window*>(glfwGetWindowUserPointer(w))->createFocusEvent(focused);
	};

	glfwSetKeyCallback(m_window, keyCallback);
	glfwSetMouseButtonCallback(m_window, mouseCallback);
	glfwSetCursorPosCallback(m_window, cursorPosCallback);
	glfwSetScrollCallback(m_window, mouseScrollCallback);
	glfwSetWindowCloseCallback(m_window, windowCloseCallback);
	glfwSetFramebufferSizeCallback(m_window, windowResizeCallback);
	glfwSetWindowFocusCallback(m_window, windowFocusCallback);

	GLenum err = glewInit();
	if(err != GLEW_OK)
	{
		glfwTerminate();
		std::stringstream sstream;
		sstream << "GLEW Error: " << glewGetErrorString(err) << '\n';

		throw std::runtime_error(sstream.str());
	}

	ImGui::CreateContext();
	ImguiImpl::init(m_window);
	ImguiImpl::newFrame();
}

poc::Window::~Window() {
	if (m_window) {
		glfwDestroyWindow(m_window);
	}
}

bool poc::Window::isOpen() const {
	return m_isOpen;
}

void poc::Window::close() {
	if (m_isOpen) {
		m_isOpen = false;
		glfwSetWindowShouldClose(m_window, GLFW_TRUE);
		glfwDestroyWindow(m_window);
		m_window = nullptr;

		m_events.clear();
	}
}

bool poc::Window::pollEvent(poc::Event& event) {
	if (m_events.empty()) {
		return false;
	}

	event = m_events.front();
	m_events.pop_front();

	return true;
}

void poc::Window::display() {
	ImguiImpl::render();

	glfwPollEvents();
	m_isOpen = !static_cast<bool>(glfwWindowShouldClose(m_window));

	glfwSwapBuffers(m_window);

	if (m_isOpen) {
		ImguiImpl::newFrame();
	}
}

void poc::Window::createKeyEvent(int key, int, int action, int mods) {
	Event event;

	switch (action) {
		case GLFW_PRESS:
			event.type = EventType::KeyPressed;
			break;
		case GLFW_RELEASE:
			event.type = EventType::KeyReleased;
			break;
		case GLFW_REPEAT:
			event.type = EventType::KeyRepeat;
			break;
		default:
			break;
	}

	Event::KeyEvent keyEvent;
	keyEvent.code = static_cast<Keyboard::Key>(key);
	keyEvent.shift = static_cast<bool>(mods & GLFW_MOD_SHIFT);
	keyEvent.control = static_cast<bool>(mods & GLFW_MOD_CONTROL);
	keyEvent.alt = static_cast<bool>(mods & GLFW_MOD_ALT);
	keyEvent.system = static_cast<bool>(mods & GLFW_MOD_SUPER);

	event.content = keyEvent;

	m_events.push_back(std::move(event));
}

void poc::Window::createMouseEvent(int button, int action, int) {
	Event event;

	switch (action) {
		case GLFW_PRESS:
			event.type = EventType::MouseButtonPressed;
			break;
		case GLFW_RELEASE:
			event.type = EventType::MouseButtonReleased;
			break;
		default:
			break;
	}

	Event::MouseButtonEvent mouseEvent;
	mouseEvent.button = static_cast<Mouse::Button>(button);

	double xpos = -1.;
	double ypos = -1.;
	glfwGetCursorPos(m_window, &xpos, &ypos);

	mouseEvent.x = static_cast<int>(xpos);
	mouseEvent.y = static_cast<int>(ypos);

	event.content = mouseEvent;

	m_events.push_back(std::move(event));
}

void poc::Window::createCloseEvent() {
	Event event;
	event.type = EventType::Closed;

	m_events.push_back(std::move(event));
}

void poc::Window::createMouseEvent(double xPos, double yPos) {
	Event event;
	event.type = EventType::MouseMoved;

	Event::MouseMoveEvent mouseEvent;
	mouseEvent.x = static_cast<int>(xPos);
	mouseEvent.y = static_cast<int>(yPos);

	event.content = mouseEvent;

	m_events.push_back(std::move(event));
}

void poc::Window::createMouseScrollEvent(double xOffset, double yOffset) {
	Event event;
	event.type = EventType::MouseScrollEvent;

	Event::MouseScrollEvent scrollEvent;
	scrollEvent.xDelta = xOffset;
	scrollEvent.yDelta = yOffset;

	event.content = scrollEvent;

	m_events.push_back(std::move(event));
}

void poc::Window::createResizeEvent(int width, int height) {
	Event event;
	event.type = EventType::Resized;

	Event::ResizeEvent resizeEvent;
	resizeEvent.width = width;
	resizeEvent.height = height;

	event.content = resizeEvent;

	m_events.push_back(std::move(event));
}

void poc::Window::createFocusEvent(int focused) {
	Event event;
	event.type = static_cast<bool>(focused) ? EventType::FocusGain : EventType::FocusLost;

	m_events.push_back(std::move(event));
}

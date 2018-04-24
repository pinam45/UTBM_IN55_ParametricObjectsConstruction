#ifndef PARAMETRICOBJECTSCONSTRUCTION_WINDOW_HPP
#define PARAMETRICOBJECTSCONSTRUCTION_WINDOW_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <deque>
#include <string_view>

#include <Event.hpp>
#include <VideoMode.hpp>
#include <ContextSettings.hpp>

namespace poc {
	enum class FullscreenMode : int {
		NoFullScreen,
		WindowedFullScreen
	};

	class Window {
	public:
		Window(const poc::VideoMode& videoMode, std::string_view title,
		       FullscreenMode fullscreenMode = FullscreenMode::NoFullScreen,
		       const poc::ContextSettings& contextSettings = ContextSettings());

		Window(const Window&) = delete;
		Window(Window&&) = default;

		Window& operator=(const Window&) = delete;
		Window& operator=(Window&&) = default;

		~Window();

		bool isOpen() const;

		void close();

		bool pollEvent(Event& event);

		void display();

	private:
		GLFWwindow* m_window;
		bool m_isOpen = false;
		std::deque<Event> m_events;

	private:
		void createKeyEvent(int key, int scancode, int action, int mods);

		void createCharEvent(unsigned int character);

		void createMouseEvent(int button, int action, int mods);

		void createMouseEvent(double xPos, double yPos);

		void createMouseScrollEvent(double xOffset, double yOffset);

		void createCloseEvent();

		void createResizeEvent(int width, int height);

		void createFocusEvent(int focused);
	};
}

#endif //PARAMETRICOBJECTSCONSTRUCTION_WINDOW_HPP

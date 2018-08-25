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

#ifndef PARAMETRICOBJECTSCONSTRUCTION_WINDOW_HPP
#define PARAMETRICOBJECTSCONSTRUCTION_WINDOW_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <deque>
#include <string_view>

#include "Event.hpp"
#include "VideoMode.hpp"
#include "ContextSettings.hpp"

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

		int getWidth() const;

		int getHeigth() const;

		void setInputMode(int mode, int value);

		bool isPressed(Keyboard::Key key) const;

		bool isPressed(Mouse::Button button) const;

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

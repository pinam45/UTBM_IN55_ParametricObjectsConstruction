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

#ifndef PARAMETRICOBJECTSCONSTRUCTION_EVENT_HPP
#define PARAMETRICOBJECTSCONSTRUCTION_EVENT_HPP

#include <variant>

#include "Keyboard.hpp"
#include "Mouse.hpp"

namespace poc {
	enum class EventType : int {
		Unknown,
		Closed,
		Resized,
		KeyPressed,
		KeyRepeat,
		KeyReleased,
		TextEntered,
		MouseMoved,
		MouseButtonPressed,
		MouseButtonReleased,
		MouseScrollEvent,
		FocusGain,
		FocusLost
	};

	class Event {
	public:
		Event() noexcept = default;

		template <typename ContentType, typename... Args>
		Event(EventType type, std::in_place_type_t<ContentType>, Args&&... contentArgs) noexcept(noexcept(ContentType{std::forward<Args>(contentArgs)...}));

		explicit constexpr Event(EventType type) noexcept;

		EventType type = EventType::Unknown;

		struct KeyEvent {
			KeyEvent() noexcept = default;
			KeyEvent(Keyboard::Key code, bool alt, bool control, bool shift, bool system) noexcept;

			Keyboard::Key code = Keyboard::Key::Unknown;
			bool alt = false;
			bool control = false;
			bool shift = false;
			bool system = false;
		};

		struct TextEvent {
			TextEvent() noexcept = default;
			explicit TextEvent(unsigned int character) noexcept;

			unsigned int character = 0;
		};

		struct MouseButtonEvent {
			MouseButtonEvent() noexcept = default;
			MouseButtonEvent(Mouse::Button button, int x, int y) noexcept;

			Mouse::Button button = Mouse::Button::Unknown;
			int x = -1;
			int y = -1;
		};

		struct MouseMoveEvent {
			MouseMoveEvent() noexcept = default;
			MouseMoveEvent(int x, int y) noexcept;

			int x = -1;
			int y = -1;
		};

		struct MouseScrollEvent {
			MouseScrollEvent() noexcept = default;
			MouseScrollEvent(double xDelta, double yDelta) noexcept;

			double xDelta = 0.;
			double yDelta = 0.;
		};

		struct ResizeEvent {
			ResizeEvent() noexcept = default;
			ResizeEvent(int width, int height);

			int width = 0;
			int height = 0;
		};

		std::variant<std::monostate, KeyEvent, TextEvent,
				MouseButtonEvent, MouseMoveEvent, MouseScrollEvent,
				ResizeEvent> content = std::monostate{};
	};

	template <typename ContentType, typename... Args>
	Event::Event(EventType type_, std::in_place_type_t<ContentType> ipt, Args&&... contentArgs) noexcept(noexcept(ContentType{std::forward<Args>(contentArgs)...}))
		: type(type_)
		, content(ipt, std::forward<Args>(contentArgs)...) {

	}

	constexpr poc::Event::Event(EventType type_) noexcept
			: type(type_) {}
}


#endif //PARAMETRICOBJECTSCONSTRUCTION_EVENT_HPP

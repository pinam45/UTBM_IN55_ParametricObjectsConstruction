#ifndef PARAMETRICOBJECTSCONSTRUCTION_EVENT_HPP
#define PARAMETRICOBJECTSCONSTRUCTION_EVENT_HPP

#include <variant>

#include <Keyboard.hpp>
#include <Mouse.hpp>
#include "Mouse.hpp"

namespace poc {
	enum class EventType : int {
		Unknown,
		Closed,
		Resized,
		KeyPressed,
		KeyRepeat,
		KeyReleased,
		MouseMoved,
		MouseButtonPressed,
		MouseButtonReleased,
		MouseScrollEvent,
		FocusGain,
		FocusLost
	};

	class Event {
	public:
		EventType type = EventType::Unknown;

		struct KeyEvent {
			Keyboard::Key code;
			bool alt;
			bool control;
			bool shift;
			bool system;
		};

		struct MouseButtonEvent {
			Mouse::Button button;
			int x;
			int y;
		};

		struct MouseMoveEvent {
			int x;
			int y;
		};

		struct MouseScrollEvent {
			double xDelta;
			double yDelta;
		};

		struct ResizeEvent {
			int width;
			int height;
		};

		std::variant<std::monostate, KeyEvent,
				MouseButtonEvent, MouseMoveEvent, MouseScrollEvent,
				ResizeEvent> content = std::monostate{};
	};
}


#endif //PARAMETRICOBJECTSCONSTRUCTION_EVENT_HPP

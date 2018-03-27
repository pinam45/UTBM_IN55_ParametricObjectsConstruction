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

		constexpr Event(EventType type) noexcept;

		EventType type = EventType::Unknown;

		struct KeyEvent {
			KeyEvent() noexcept = default;
			KeyEvent(Keyboard::Key code, bool alt, bool control, bool shift, bool system) noexcept;

			Keyboard::Key code = Keyboard::Key::Unknown;
			bool alt;
			bool control;
			bool shift;
			bool system;
		};

		struct TextEvent {
			TextEvent() noexcept = default;
			TextEvent(unsigned int character_) noexcept;

			unsigned int character;
		};

		struct MouseButtonEvent {
			MouseButtonEvent() noexcept = default;
			MouseButtonEvent(Mouse::Button button_, int x_, int y_) noexcept;

			Mouse::Button button;
			int x;
			int y;
		};

		struct MouseMoveEvent {
			MouseMoveEvent() noexcept = default;
			MouseMoveEvent(int x_, int y_) noexcept;

			int x;
			int y;
		};

		struct MouseScrollEvent {
			MouseScrollEvent() noexcept = default;
			MouseScrollEvent(double xDelta, double yDelta) noexcept;

			double xDelta;
			double yDelta;
		};

		struct ResizeEvent {
			ResizeEvent() noexcept = default;
			ResizeEvent(int width, int height);

			int width;
			int height;
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

	constexpr poc::Event::Event(EventType type) noexcept
			: type(type) {}
}


#endif //PARAMETRICOBJECTSCONSTRUCTION_EVENT_HPP

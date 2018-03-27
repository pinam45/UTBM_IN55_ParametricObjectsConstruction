#include <Event.hpp>

poc::Event::KeyEvent::KeyEvent(Keyboard::Key code_, bool alt_, bool control_, bool shift_, bool system_) noexcept
		: code{code_}
		, alt{alt_}
		, control{control_}
		, shift{shift_}
		, system{system_} {

}

poc::Event::TextEvent::TextEvent(unsigned int character_) noexcept
		: character{character_} {
}

poc::Event::MouseButtonEvent::MouseButtonEvent(poc::Mouse::Button button_, int x_, int y_) noexcept
		: button(button_)
		, x(x_)
		, y(y_) {
}

poc::Event::MouseMoveEvent::MouseMoveEvent(int x_, int y_) noexcept
		: x(x_)
		, y(y_) {
}

poc::Event::MouseScrollEvent::MouseScrollEvent(double xDelta, double yDelta) noexcept
		: xDelta(xDelta),
		  yDelta(yDelta) {
}

poc::Event::ResizeEvent::ResizeEvent(int width, int height)
		: width(width)
		, height(height) {
}

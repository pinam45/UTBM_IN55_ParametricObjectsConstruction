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

#include "Event.hpp"

poc::Event::KeyEvent::KeyEvent(Keyboard::Key code_, bool alt_, bool control_, bool shift_, bool system_) noexcept
  : code{code_}
  , alt{alt_}
  , control{control_}
  , shift{shift_}
  , system(system_) {

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

poc::Event::MouseScrollEvent::MouseScrollEvent(double xDelta_, double yDelta_) noexcept
  : xDelta(xDelta_), yDelta(yDelta_) {
}

poc::Event::ResizeEvent::ResizeEvent(int width_, int height_)
  : width(width_)
  , height(height_) {
}

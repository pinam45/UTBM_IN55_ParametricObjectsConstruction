#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <variant>

#include <VideoMode.hpp>
#include <GlfwInitializer.hpp>
#include <callbacks.hpp>
#include <Window.hpp>
#include <Event.hpp>

constexpr int DEFAULT_WIDTH = 800;
constexpr int DEFAULT_HEIGHT = 600;

int main()
{
	poc::GlfwInitializer initializer;
	poc::VideoMode test = poc::VideoMode::getPrimaryMonitorVideoMode();
	poc::Window w(test, "Test", poc::FullscreenMode::NoFullScreen);

	GLenum err = glewInit();
	if(err != GLEW_OK)
	{
		std::cerr << "GLEW error: " << glewGetErrorString(err) << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glViewport(0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT);

	while(w.isOpen())
	{
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		poc::Event event;

		while (w.pollEvent(event)) {

			std::visit([&event, &w](auto&& content) noexcept {
				using T = std::decay_t<decltype(content)>;

				if constexpr (std::is_same_v<T, poc::Event::KeyEvent>) {
					if (event.type == poc::EventType::KeyReleased) {
						std::cout << "Key Released" << static_cast<int>(content.code) << '\n';
						if (content.code == poc::Keyboard::Key::Escape) {
							w.close();
						}
					} else if (event.type == poc::EventType::KeyPressed) {
						std::cout << "Key Pressed" << static_cast<int>(content.code) << '\n';
					} else if (event.type == poc::EventType::KeyRepeat) {
						std::cout << "Key Repeat" << static_cast<int>(content.code) << '\n';
					}
				} else if constexpr (std::is_same_v<T, poc::Event::MouseButtonEvent>) {
					if (event.type == poc::EventType::MouseButtonPressed) {
						std::cout << "Mouse Pressed, x=" << content.x << " , y=" << content.y << " , button=" << static_cast<int>(content.button) << '\n';
					}
				} else if constexpr (std::is_same_v<T, poc::Event::ResizeEvent>) {
					std::cout << "Resize Event, x=" << content.width << " , y=" << content.height << '\n';
				} else if (event.type == poc::EventType::FocusGain) {
					std::cout << "Focus Gain\n";
				} else if (event.type == poc::EventType::FocusLost) {
					std:: cout << "Focus Lost\n";
				}

			}, event.content);
		}
		// nothing

		w.display();
	}

	return EXIT_SUCCESS;
}

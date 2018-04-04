#include <iostream>
#include <variant>
#include <codecvt>
#include <locale>

#include <VideoMode.hpp>
#include <GlfwInitializer.hpp>
#include <Window.hpp>
#include <Event.hpp>
#include <Shader.hpp>
#include <ShaderProgram.hpp>

#include <imgui.h>

constexpr int DEFAULT_WIDTH = 800;
constexpr int DEFAULT_HEIGHT = 600;

//#pragma clang diagnostic push
//#pragma ide diagnostic ignored "OCSimplifyInspection"
//#pragma ide diagnostic ignored "OCDFAInspection"
int main()
{
	poc::GlfwInitializer initializer;

	poc::VideoMode test = poc::VideoMode::getPrimaryMonitorVideoMode();
	poc::Window w(test, "Test", poc::FullscreenMode::NoFullScreen);

	glViewport(0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT);

	ImGui::StyleColorsDark();

	constexpr float vertices[] = {
	  0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
	  -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
	  0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f
	};

	unsigned int VAO = 0; // Vertex Array Object
	unsigned int VBO = 0; // Vertex Buffer Object
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	Shader vshader = Shader::fromFile(GL_VERTEX_SHADER, "shaders/vertex.glsl");
	if(!vshader.isValid()){
		std::cout << "Vertex shader error:\n" << vshader.getError() << std::endl;
		return EXIT_FAILURE;
	}

	Shader fshader = Shader::fromFile(GL_FRAGMENT_SHADER, "shaders/fragment.glsl");
	if(!fshader.isValid()){
		std::cout << "Fragment shader error:\n" << fshader.getError() << std::endl;
		return EXIT_FAILURE;
	}

	ShaderProgram program(vshader, fshader);
	if(!program.isValid()){
		std::cout << "Shader program error:\n" << program.getError() << std::endl;
		return EXIT_FAILURE;
	}
	program.use();

	bool open = true;
	while(w.isOpen())
	{
		glClearColor(.05f, .05f, .05f, .05f);
		glClear(GL_COLOR_BUFFER_BIT);

		program.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

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
				} else if constexpr (std::is_same_v<T, poc::Event::TextEvent>) {
					std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
					std::string u8str = converter.to_bytes(content.character);
					std::cout << "Text Event, char='" << u8str << "'\n";
				} else if (event.type == poc::EventType::FocusGain) {
					std::cout << "Focus Gain\n";
				} else if (event.type == poc::EventType::FocusLost) {
					std:: cout << "Focus Lost\n";
				} else if (event.type == poc::EventType::Closed) {
					std::cout << "Closed\n";
				}

			}, event.content);
		}

		ImGui::ShowDemoWindow(&open);
		w.display();
	}

	return EXIT_SUCCESS;
}
//#pragma clang diagnostic pop

#include <iostream>
#include <variant>
#include <codecvt>
#include <locale>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>

#include <glm/trigonometric.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include "VideoMode.hpp"
#include "GlfwInitializer.hpp"
#include "Window.hpp"
#include "Event.hpp"
#include "Shader.hpp"
#include "ShaderProgram.hpp"
#include "Camera.hpp"
#include "SphericalCamera.hpp"
#include "ParametricObject.hpp"

#include "gui/Drawable.hpp"
#include "gui/FPSOverlay.hpp"
#include "gui/POConfigPanel.hpp"

constexpr int DEFAULT_WIDTH = 800;
constexpr int DEFAULT_HEIGHT = 600;

constexpr float MOUSE_MOVE_SENSITIVITY = 0.007f;
constexpr float MOUSE_SCROLL_SENSITIVITY = 0.5f;

constexpr int PANEL_WIDTH = 300;

void check_error();

void check_error() {
    GLenum  err;
    err =  glGetError();
    if (err != GL_NO_ERROR) {
        std::cout << err << std::endl;
        std::abort();
    }
}

int main()
{
	poc::GlfwInitializer initializer;

	poc::VideoMode test(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	poc::Window w(test, "Test", poc::FullscreenMode::NoFullScreen);

	poc::FPSOverlay fpsOverlay;

	ImGui::StyleColorsDark();

	glEnable(GL_DEPTH_TEST);
	w.setInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	poc::Shader vshader = poc::Shader::fromFile(GL_VERTEX_SHADER, "shaders/vertex.glsl");
	if(!vshader.isValid()){
		std::cout << "Vertex shader error:\n" << vshader.getError() << std::endl;
		return EXIT_FAILURE;
	}

	poc::Shader fshader = poc::Shader::fromFile(GL_FRAGMENT_SHADER, "shaders/fragment.glsl");
	if(!fshader.isValid()){
		std::cout << "Fragment shader error:\n" << fshader.getError() << std::endl;
		return EXIT_FAILURE;
	}

	poc::ShaderProgram program(vshader, fshader);
	if(!program.isValid()){
		std::cout << "Shader program error:\n" << program.getError() << std::endl;
		return EXIT_FAILURE;
	}
	program.use();

	const glm::vec3 camera_focus(0.f, 0.f, 0.f);

	poc::SphericalCamera camera(glm::radians(45.0f), DEFAULT_WIDTH, DEFAULT_HEIGHT, 0.1f, 100.0f, camera_focus, {0.f, 0.f, -7.f});
	camera.update();

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 0.0f;
	style.ScrollbarRounding = 0.0f;

	poc::POConfigPanel panel(0, 0, static_cast<float>(PANEL_WIDTH), static_cast<float>(w.getHeigth()));
	poc::ParametricObject parametricObject = poc::ParametricObject(panel.getLayers());

	unsigned int VAO = 0; // Vertex Array Object
	unsigned int VBO = 0; // Vertex Buffer Object
	unsigned int EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, static_cast<int>(parametricObject.getNbPoint() * 6 * sizeof(float)), parametricObject.getVertices().data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<int>(parametricObject.getNbIndexes() * sizeof(unsigned int)), parametricObject.getIndexes().data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	bool first_mouse_move = true;
	float mouse_last_x;
	float mouse_last_y;
	bool mouse_drag =false;

	bool esc_pressed = false;
    while(w.isOpen())
	{
		poc::Event event;
		while (w.pollEvent(event)) {

			std::visit([&](auto&& content) noexcept {
				using T = std::decay_t<decltype(content)>;

				if constexpr (std::is_same_v<T, poc::Event::KeyEvent>) {
					if (event.type == poc::EventType::KeyPressed) {
						if (content.code == poc::Keyboard::Key::Escape){
							esc_pressed = true;
						}
					}
				}

				if constexpr (std::is_same_v<T, poc::Event::MouseButtonEvent>) {
					if(content.button == poc::Mouse::Button::Button1){
						if(event.type == poc::EventType::MouseButtonPressed){
							mouse_drag = true;
						}
						if(event.type == poc::EventType::MouseButtonReleased){
							mouse_drag = false;
							first_mouse_move = true;
						}
					}
				}

				if constexpr (std::is_same_v<T, poc::Event::MouseMoveEvent>) {
					if(mouse_drag){
						if(first_mouse_move)
						{
							mouse_last_x = static_cast<float>(content.x);
							mouse_last_y = static_cast<float>(content.y);
							first_mouse_move = false;
							return;
						}

						const float xoffset = static_cast<float>(content.x) - mouse_last_x;
						const float yoffset = static_cast<float>(content.y) - mouse_last_y;
						mouse_last_x = static_cast<float>(content.x);
						mouse_last_y = static_cast<float>(content.y);

						camera.moveRight(-MOUSE_MOVE_SENSITIVITY * xoffset);
						camera.moveUp(-MOUSE_MOVE_SENSITIVITY * yoffset);
					}
				}

				if constexpr (std::is_same_v<T, poc::Event::MouseScrollEvent>){
					camera.moveForward(static_cast<float>(content.yDelta) * MOUSE_SCROLL_SENSITIVITY);
				}
			}, event.content);
		}

		glViewport(PANEL_WIDTH, 0, w.getWidth() - PANEL_WIDTH, w.getHeigth());

		camera.setWidth(static_cast<float>(w.getWidth() - PANEL_WIDTH));
		camera.setHeight(static_cast<float>(w.getHeigth()));
		camera.update();

		program.setUniformMatrix4v("view", 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
		program.setUniformMatrix4v("projection", 1, GL_FALSE, glm::value_ptr(camera.getProjectionMatrix()));

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		program.use();
		glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		glEnable(GL_PROGRAM_POINT_SIZE);
        //glDisable(GL_CULL_FACE);

		glDrawElements(GL_TRIANGLES , static_cast<int>(parametricObject.getNbIndexes()), GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);

		glViewport(0,0,w.getWidth(),w.getHeigth());
		fpsOverlay.draw();

		panel.setHeight(static_cast<float>(w.getHeigth()));
		if(panel.draw()){
			parametricObject.configure(panel.getLayers());
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, static_cast<int>(parametricObject.getNbPoint() * 6 * sizeof(float)), parametricObject.getVertices().data(), GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<int>(parametricObject.getNbIndexes() * sizeof(unsigned int)), parametricObject.getIndexes().data(), GL_STATIC_DRAW);
		}

		w.display();

		if(esc_pressed)
			w.close();
	}

	return EXIT_SUCCESS;
}

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
#include <Camera.hpp>

#include <imgui.h>
#include <glm/trigonometric.hpp>
#include <glm/gtc/type_ptr.hpp>

constexpr int DEFAULT_WIDTH = 800;
constexpr int DEFAULT_HEIGHT = 600;

//#pragma clang diagnostic push
//#pragma ide diagnostic ignored "OCSimplifyInspection"
//#pragma ide diagnostic ignored "OCDFAInspection"
int main()
{
	poc::GlfwInitializer initializer;

	poc::VideoMode test(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	poc::Window w(test, "Test", poc::FullscreenMode::NoFullScreen);

	//glViewport(0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT);

	//ImGui::StyleColorsDark();

	glEnable(GL_DEPTH_TEST);
	w.setInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	float vertices[] = {
	  -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
	  0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
	  0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
	  0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
	  -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
	  -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,

	  -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
	  0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
	  0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
	  0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
	  -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
	  -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,

	  -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
	  -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
	  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
	  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
	  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
	  -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,

	  0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
	  0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
	  0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
	  0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
	  0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
	  0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,

	  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
	  0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
	  0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
	  0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
	  -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
	  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,

	  -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
	  0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
	  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
	  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
	  -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
	  -0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 1.0f,
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

	Camera camera(glm::radians(45.0f), DEFAULT_WIDTH, DEFAULT_HEIGHT, 0.1f, 100.0f);
	camera.setPosition({0,0,-3});
	camera.lookAt({0,0,0});

	float delta_time = 0.0f;
	float last_frame = 0.0f;
	bool first_mouse_move = true;
	float mouse_last_x;
	float mouse_last_y;
	//bool open = true;
	bool esc_pressed = false;
	while(w.isOpen())
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		delta_time = currentFrame - last_frame;
		last_frame = currentFrame;

		poc::Event event;
		while (w.pollEvent(event)) {

			std::visit([&](auto&& content) noexcept {
				using T = std::decay_t<decltype(content)>;

				if constexpr (std::is_same_v<T, poc::Event::KeyEvent>) {
					if (event.type == poc::EventType::KeyPressed || event.type == poc::EventType::KeyRepeat) {
						float cameraSpeed = 3.0f * delta_time;
						if (content.code == poc::Keyboard::Key::W)
							camera.moveForward(cameraSpeed);
						if (content.code == poc::Keyboard::Key::S)
							camera.moveForward(-cameraSpeed);
						if (content.code == poc::Keyboard::Key::A)
							camera.moveRight(cameraSpeed);
						if (content.code == poc::Keyboard::Key::D)
							camera.moveRight(-cameraSpeed);

						if (content.code == poc::Keyboard::Key::Escape)
							esc_pressed = true;
					}
				}

				if constexpr (std::is_same_v<T, poc::Event::MouseMoveEvent>) {
					if(first_mouse_move)
					{
						mouse_last_x = static_cast<float>(content.x);
						mouse_last_y = static_cast<float>(content.y);
						first_mouse_move = false;
						return;
					}

					float xoffset = static_cast<float>(content.x) - mouse_last_x;
					float yoffset = mouse_last_y - static_cast<float>(content.y); // reversed since y-coordinates range from bottom to top
					mouse_last_x = static_cast<float>(content.x);
					mouse_last_y = static_cast<float>(content.y);

					float sensitivity = 0.005f;
					camera.rotateHorizontally(sensitivity * xoffset);
					camera.rotateVertically(-sensitivity * yoffset);
				}
			}, event.content);
		}

		camera.setWidth(static_cast<float>(w.getWidth()));
		camera.setHeight(static_cast<float>(w.getHeigth()));
		camera.update();

		program.setUniformMatrix4v("view", 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
		program.setUniformMatrix4v("projection", 1, GL_FALSE, glm::value_ptr(camera.getProjectionMatrix()));

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		program.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		//ImGui::ShowDemoWindow(&open);
		w.display();

		if(esc_pressed)
			w.close();
	}

	return EXIT_SUCCESS;
}
//#pragma clang diagnostic pop

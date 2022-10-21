#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>

#include "shader.h"
#include "io/Keyboard.h"
#include "io/Mouse.h"

void frame_buffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

float mix_val = 0.5f;
glm::mat4 transform = glm::mat4(1.0f);
glm::mat4 mouse_transform = glm::mat4(1.0f);

float theta = 45.0f;
float x, y, z;
unsigned int WIDTH = 800, HEIGHT = 600;

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "open Gl", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, WIDTH, HEIGHT);
	glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

	glfwSetKeyCallback(window, keyboard::key_callback);

	glfwSetMouseButtonCallback(window, mouse::mouse_button_callback);
	glfwSetScrollCallback(window, mouse::mouse_wheel_callback);

	glEnable(GL_DEPTH_TEST);

	const shader shader("assets/vertex_core.glsl", "assets/fragment_core.glsl");

	//vertex array
	constexpr float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	// VAO, VBO, EBO	
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	// bind VAO - связываем
	glBindVertexArray(VAO);

	// bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);


	// Set attributes pointer
	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), static_cast<void*>(nullptr));
	glEnableVertexAttribArray(0);
	// color

	// texture coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// TEXTURE==============

	unsigned int texture1, texture2;

	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// s == x; t == y..
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// load image
	int width, height, nChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("assets/i.jpg", &width, &height, &nChannels, 0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed load image" << std::endl;

	stbi_image_free(data);

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	data = stbi_load("assets/123.jpg", &width, &height, &nChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed load texture 2" << std::endl;

	stbi_image_free(data);

	shader.activate();

	shader.set_int("texture1", 0);
	shader.set_int("texture2", 1);

	x = 0.0f;
	y = 0.0f;
	z = 3.0f;

	while (!glfwWindowShouldClose(window)) {
		// input
		process_input(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glBindVertexArray(VAO);
		shader.activate();

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		model = rotate(model, static_cast<float>(glfwGetTime()) * glm::radians(-40.0f), glm::vec3(0.5f));
		view = translate(view, glm::vec3(-x, -y, -z));
		projection = glm::perspective(glm::radians(45.0f), static_cast<float>(WIDTH) / static_cast<float>(HEIGHT), 0.1f, 100.0f);

		// draw shader
		shader.activate();

		shader.set_mat4("model", model);
		shader.set_mat4("view", view);
		shader.set_mat4("projection", projection);
		shader.set_float("mixVal", mix_val);
		shader.set_mat4("transform", transform);
		shader.set_mat4("mouse_transform", mouse_transform);

		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// Send new frame to window
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &VBO);
	glfwTerminate();
	return 0;
}

void frame_buffer_size_callback(GLFWwindow* window, const int width, const int height) {
	glViewport(0, 0, width, height);
	WIDTH = width;
	HEIGHT = height;
}

void process_input(GLFWwindow* window) {
	if (keyboard::key(GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, true);

	if (keyboard::key_went_up(GLFW_KEY_UP)) {
		mix_val += .05f;
		if (mix_val > 1) {
			mix_val = 1.0f;
		}
	}

	if (keyboard::key_went_down(GLFW_KEY_DOWN)) {
		mix_val -= .05f;
		if (mix_val < 0) {
			mix_val = 0.0f;
		}
	}

	if (keyboard::key(GLFW_KEY_W)) {
		transform = glm::translate(transform, glm::vec3(0.0f, -0.01f, 0.0f));
	}

	if (keyboard::key(GLFW_KEY_S)) {
		transform = glm::translate(transform, glm::vec3(0.0f, 0.01f, 0.0f));
	}

	if (keyboard::key(GLFW_KEY_D)) {
		transform = glm::translate(transform, glm::vec3(-0.01f, 0.0f, 0.0f));
	}

	if (keyboard::key(GLFW_KEY_A)) {
		transform = translate(transform, glm::vec3(0.01f, 0.0f, 0.0f));
	}

	// all bed
	if (mouse::button(GLFW_MOUSE_BUTTON_LEFT)) {
		float _x = mouse::get_mouse_x();
		float _y = mouse::get_mouse_y();

		std::cout << mouse::get_mouse_x() << "--" << mouse::get_mouse_y() << " ";

		mouse_transform = translate(mouse_transform, glm::vec3(0.01f, 0.0f, 0.0f));
	}

	if (mouse::button(GLFW_MOUSE_BUTTON_RIGHT)) {
		mouse_transform = glm::translate(mouse_transform, glm::vec3(-0.01f, 0.0f, 0.0f));
	}
}

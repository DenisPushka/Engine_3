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

int main() {
	//// glm test
	//glm::vec4 vec(1.0f, 1.0f, 1.0f, 1.0f);
	//glm::mat4 trans = glm::mat4(1.0f);
	//trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));							// Перевод -- translate
	//trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));			// Вращение -- rotate
	//trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));									// масштаб -- scale
	//vec = trans * vec;
	//std::cout << vec.x << ' ' <<  vec.y << ' ' << vec.z << std::endl;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(800, 600, "open Gl", nullptr, nullptr);
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

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

	glfwSetKeyCallback(window, keyboard::key_callback);
	glfwSetMouseButtonCallback(window, mouse::mouse_button_callback);
	glfwSetScrollCallback(window, mouse::mouse_wheel_callback);

	const shader shader("assets/vertex_core.glsl", "assets/fragment_core.glsl");

	//vertex array
	constexpr float vertices[] = {
		// position				color				texture coordinate
		-0.5f, -0.5f, 0.0f,		1.0f, 1.0f, 0.5f,	0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f,		0.5f, 1.0f, 0.75f,	0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f,		0.6f, 1.0f, 0.2f,	1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f,		1.0f, 0.2f, 1.0f,	1.0f, 1.0f,
	};

	const int indices[] = {
		0, 1, 2, // first triangle
		3, 1, 2  // second triangle
	};

	// VAO, VBO, EBO	
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// bind VAO - связываем
	glBindVertexArray(VAO);

	// bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);

	//set EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices, GL_STATIC_DRAW);


	// Set attributes pointer
	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), static_cast<void*>(nullptr));
	glEnableVertexAttribArray(0);
	// color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

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
	// shader.set_float("mixVal", mix_val);

	shader.activate();
	shader.set_int("texture1", 0);
	shader.set_int("texture2", 1);


	// SHADER===============
	// Transform
	//const auto trans = glm::mat4(1.0f);
	//trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//shader.activate();
	//shader.set_mat4("transform", trans);

	while (!glfwWindowShouldClose(window)) {
		// input
		process_input(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// draw shader
		glBindVertexArray(VAO);
		shader.activate();


		shader.set_float("mixVal", mix_val);
		shader.set_mat4("transform", transform);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);

		//trans = rotate(trans, glm::radians(static_cast<float>(glfwGetTime()) / 100.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		//shader.set_mat4("transform", trans);

		// Send new frame to window
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void frame_buffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window) {
	if (keyboard::key(GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, true);

	// change mix value
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
		transform = glm::translate(transform, glm::vec3(0.0f, 0.1f, 0.0f));
	}

	if (keyboard::key(GLFW_KEY_S)) {
		transform = glm::translate(transform, glm::vec3(0.0f, -0.1f, 0.0f));
	}

	if (keyboard::key(GLFW_KEY_D)) {
		transform = glm::translate(transform, glm::vec3(0.1f, 0.0f, 0.0f));
	}

	if (keyboard::key(GLFW_KEY_A)) {
		transform = glm::translate(transform, glm::vec3(-0.1f, 0.0f, 0.0f));
	}
}

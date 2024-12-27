#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glad/glad.h>

#include <iostream>

#include "../include/Settings.h"
#include "../include/Shader.h"
#include "../include/VertexBuffer.h"
#include "../include/ElementBuffer.h"
#include "../include/VertexArray.h"
#include "../include/Texture.h"

void error_callback(int error, const char* description)
{
	std::cerr << "Error: " << description << std::endl;
}

void framebuffer_resize_callback(GLFWwindow* window, int width, int height) {
	MazeEscaping::screen_width = width;
	MazeEscaping::screen_height = height;
	glViewport(0, 0, MazeEscaping::screen_width, MazeEscaping::screen_height);
}

int main() {
	if (!glfwInit()) {
		std::cerr << "Failed to init glfw" << std::endl;
		return 1;
	}

	glfwSetErrorCallback(error_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(MazeEscaping::screen_width, MazeEscaping::screen_height, MazeEscaping::window_title, nullptr, nullptr);

	if (!window) {
		std::cerr << "Failed to create a glfw window" << std::endl;
		return 1;
	}

	glfwMakeContextCurrent(window);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to init glad" << std::endl;
		return 1;
	}

	glViewport(0, 0, MazeEscaping::screen_width, MazeEscaping::screen_height);

	glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);


	// part to move from here
	VertexArray va;
	va.Bind();
	float vertexTempData[] = {
		 0.0f,  0.5f, 0.0f,  0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f,  1.0f, 1.0f
	};
	unsigned int elementBufferData[] = {
		0, 1, 2
	};
	VertexBuffer vb(vertexTempData, sizeof(vertexTempData));
	ElementBuffer eb(elementBufferData, sizeof(elementBufferData));
	// end of part to move

	Shader mainShader("resources/shaders/vertex.shader", "resources/shaders/fragment.shader");
	mainShader.Use();

	Texture texture("resources/assets/bg.png", GL_TEXTURE0, GL_RGBA);
	texture.Bind();
	std::string texturUniformName = "un_texture";
	mainShader.SetUniformInt(texturUniformName, 0);

	while (!glfwWindowShouldClose(window)) {

		glClearColor(MazeEscaping::background_color.red, MazeEscaping::background_color.green, MazeEscaping::background_color.blue, MazeEscaping::background_color.alpha);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
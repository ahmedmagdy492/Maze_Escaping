#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glad/glad.h>

#include <iostream>

#include "../include/Settings.h"
#include "../include/MazeRenderer.h"

bool keys[1024];
glm::vec3 cameraFront;
float lastX = 400.0f, lastY = 300.0f;
float pitch = 0.0f, yaw = 0.0f;

void error_callback(int error, const char* description)
{
	std::cerr << "Error: " << description << std::endl;
}

void framebuffer_resize_callback(GLFWwindow* window, int width, int height) {
	MazeEscaping::screen_width = width;
	MazeEscaping::screen_height = height;
	glViewport(0, 0, MazeEscaping::screen_width, MazeEscaping::screen_height);
}

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (action == GLFW_PRESS) {
		keys[key] = true;
	}
	else if (action == GLFW_RELEASE) {
		keys[key] = false;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos;

	float sensitivity = 0.2f;

	xOffset *= sensitivity;
	yOffset *= sensitivity;

	yaw += xOffset;
	pitch += yOffset;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	cameraFront.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	cameraFront.y = sin(glm::radians(pitch));
	cameraFront.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cameraFront = glm::normalize(cameraFront);

	lastX = xpos;
	lastY = ypos;
}

int main() {
	float deltaTime = 0.0f;
	float lastFrameTime = 0.0f;
	GLFWwindow* window = nullptr;

	if (!glfwInit()) {
		std::cerr << "Failed to init glfw" << std::endl;
		return 1;
	}

	glfwSetErrorCallback(error_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(MazeEscaping::screen_width, MazeEscaping::screen_height, MazeEscaping::window_title, nullptr, nullptr);

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
	glEnable(GL_DEPTH_TEST);

	glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);

	glfwSetKeyCallback(window, (GLFWkeyfun)keyboard_callback);
	glfwSetCursorPosCallback(window, (GLFWcursorposfun)mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	MazeRenderer mazeRenderer(MazeEscaping::screen_width, MazeEscaping::screen_height);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrameTime;
		lastFrameTime = currentTime;

		mazeRenderer.Update(cameraFront, keys, deltaTime);

		mazeRenderer.RenderMaze();
		
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
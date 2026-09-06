#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void windowInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, unsigned int width, unsigned int height) {
	if (width == 0 || height == 0) return;
	glViewport(0, 0, width, height);
}

int main(){
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Medieval RPG", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	if (!window) {
		std::cerr << "Failed to initialize window" << std::endl;
		return -2;
	}

	if (!gladLoadGL()) {
		std::cerr << "Failed to load GLAD" << std::endl;
		return -3;
	}

	while (!glfwWindowShouldClose(window)) {
		windowInput(window);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

#ifndef  INPUT_CLASS_H
#define INPUT_CLASS_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Input {
public:
	Input(GLFWwindow* window) {};

	void windowInput(GLFWwindow* window, glm::vec3& camPos, const glm::vec3& camFront, float deltaTime) {
		// Default walking speed
		float speed = 2.5f * deltaTime;

		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 right = glm::normalize(glm::cross(camFront, up));

		// Escape closes the game
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

		// Running frontwards is faster than going backwards or sidewards
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) speed = 3.0f * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) speed = 5.0f * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)) speed = 3.5f * deltaTime;
		
		// Crouching makes you slower
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) speed = 1.0f * deltaTime;

		// Default
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camPos += camFront * speed;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camPos -= camFront * speed;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camPos -= right * speed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camPos += right * speed;
	}
};

#endif

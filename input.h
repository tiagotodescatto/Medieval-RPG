#ifndef  INPUT_CLASS_H
#define INPUT_CLASS_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Input {
public:
	Input(GLFWwindow* window) {};

	void windowInput(GLFWwindow* window, glm::vec3& camPos, float deltaTime) {
		float speed = 2.5f * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camPos.z -= speed;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camPos.z += speed;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camPos.x -= speed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camPos.x += speed;
	}
};

#endif

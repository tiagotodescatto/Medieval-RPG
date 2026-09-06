#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shaders/shaderclass.h"
#include "input.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, unsigned int width, unsigned int height) {
	if (width == 0 || height == 0) return;
	glViewport(0, 0, width, height);
}

// MADE WITH AI
float vertices[] = {
    // posição            // normal
    // Face traseira (-Z)
    -0.5f, -0.5f, -0.5f, //   0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f, //  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f, //  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f, //  0.0f,  0.0f, -1.0f,

    // Face frontal (+Z)
    -0.5f, -0.5f,  0.5f, //  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f, //  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f, //  0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f, //  0.0f,  0.0f,  1.0f,

    // Face esquerda (-X)
    -0.5f,  0.5f,  0.5f, // -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, // -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, // -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, // -1.0f,  0.0f,  0.0f,

    // Face direita (+X)
     0.5f,  0.5f,  0.5f, //  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f, //  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f, //  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f, //  1.0f,  0.0f,  0.0f,

     // Face inferior (-Y)
     -0.5f, -0.5f, -0.5f, //  0.0f, -1.0f,  0.0f,
      0.5f, -0.5f, -0.5f, //  0.0f, -1.0f,  0.0f,
      0.5f, -0.5f,  0.5f, //  0.0f, -1.0f,  0.0f,
     -0.5f, -0.5f,  0.5f, //  0.0f, -1.0f,  0.0f,

     // Face superior (+Y)
     -0.5f,  0.5f, -0.5f, //  0.0f,  1.0f,  0.0f,
      0.5f,  0.5f, -0.5f, //  0.0f,  1.0f,  0.0f,
      0.5f,  0.5f,  0.5f, //  0.0f,  1.0f,  0.0f,
     -0.5f,  0.5f,  0.5f, //  0.0f,  1.0f,  0.0f,
};

unsigned int indices[] = {
    0, 1, 2,   2, 3, 0,   // traseira
    4, 5, 6,   6, 7, 4,   // frontal
    4, 0, 3,   3, 7, 4,   // esquerda
    1, 5, 6,   6, 2, 1,   // direita
    4, 5, 1,   1, 0, 4,   // inferior
    3, 2, 6,   6, 7, 3,   // superior
};

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

    glEnable(GL_DEPTH_TEST);

    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

	Shader shader("shaders/vertexshader.vert", "shaders/fragmentshader.frag");
	shader.use();

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

	Input input(window);

	while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

		input.windowInput(window, cameraPos, deltaTime);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		shader.use();

        glm::mat4 model = glm::mat4(1.0f); // cube on the origin of the scene
        glm::mat4 view = glm::lookAt(
            cameraPos,
            cameraPos + glm::vec3(0, 0, -1),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );
        glm::mat4 projection = glm::perspective(
            glm::radians(60.0f), // FOV
            1280.0f / 720.0f, // Aspect Ratio
            0.1f, // Min render distance
            1000.0f // Max render distance
        );

        int modelLoc = glGetUniformLocation(shader.ID, "model");
        int viewLoc = glGetUniformLocation(shader.ID, "view");
        int projLoc = glGetUniformLocation(shader.ID, "projection");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

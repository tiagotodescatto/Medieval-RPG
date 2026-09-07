#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaders/shaderclass.h"
#include "input.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Sets precision
#include <iomanip>

void framebuffer_size_callback(GLFWwindow* window,int width, int height) {
	if (width == 0 || height == 0) return;
	glViewport(0, 0, width, height);
}

// MADE WITH AI
float vertices[] = {
    // posição              // normal            
    // Face traseira (-Z)
    -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,

    // Face frontal (+Z)
    -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,

    // Face esquerda (-X)
    -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,

    // Face direita (+X)
     0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,

     // Face inferior (-Y)
     -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,
      0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,
      0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,
     -0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,

     // Face superior (+Y)
     -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,
      0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,
      0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,
     -0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,
};

unsigned int indices[] = {
    0, 1, 2,   2, 3, 0,   // traseira
    4, 5, 6,   6, 7, 4,   // frontal
    4, 0, 3,   3, 7, 4,   // esquerda
    1, 5, 6,   6, 2, 1,   // direita
    4, 5, 1,   1, 0, 4,   // inferior
    3, 2, 6,   6, 7, 3,   // superior
};

float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 1280.0f / 2.0f;
float lastY = 720.0f / 2.0f;
bool firstMouse = true;

glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 lastPos = glm::vec3(0.0f, 0.0f, 3.0f);

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn){
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    float sensibility = 0.1f;
    xoffset *= sensibility;
    yoffset *= sensibility;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
}

void coordinates_callback() {
    if (cameraPos != lastPos) {
        std::cout << "X: " << std::fixed << std::setprecision(2) << cameraPos.x  << " ----- " << "Y: " << std::fixed << std::setprecision(2) << cameraPos.y << " ----- " << "Z: " << std::fixed << std::setprecision(2) << cameraPos.z << std::endl;
    }
    lastPos = cameraPos;
}

int main(){
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	GLFWwindow* window = glfwCreateWindow(1280.0f, 720.0f, "Medieval RPG", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	if (!window) {
		std::cerr << "Failed to initialize window" << std::endl;
		return -2;
	}

	if (!gladLoadGL()) {
		std::cerr << "Failed to load GLAD" << std::endl;
		return -3;
	}

    // makes resizable
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glEnable(GL_DEPTH_TEST);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    GLuint vao, vbo, ebo, textvao, textvbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // layout (location = 0) => cube coords
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // layout (location = 1) => normal coords
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

	Shader shader("shaders/vertexshader.vert", "shaders/fragmentshader.frag");
	shader.use();

    Shader textShader("shaders/textshader.vert", "shaders/textshader.frag");
    textShader.use();

    glm::vec3 lastCameraPos = cameraPos;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float rotationAngle = 0.0f;

	Input input(window);

	while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        rotationAngle += 60.0f * deltaTime;

        // Input
		input.windowInput(window, cameraPos, cameraFront, deltaTime);

        // Clear Screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        // Cube
		shader.use();

        glm::mat4 model = glm::mat4(1.0f); // cube on the origin of the scene
        model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 view = glm::lookAt(
            cameraPos,
            cameraPos + cameraFront,
            glm::vec3(0.0f, 1.0f, 0.0f)
        );
        glm::mat4 projection = glm::perspective(
            glm::radians(60.0f), // FOV
            16.0f / 9.0f, // Aspect Ratio
            0.1f, // Min render distance
            1000.0f // Max render distance
        );

        // Provisory ground colision
        if (cameraPos.y < 0) cameraPos.y = 0;

        // Print coordinates
        coordinates_callback();

        // Uniforms
        int modelLoc = glGetUniformLocation(shader.ID, "model");
        int viewLoc = glGetUniformLocation(shader.ID, "view");
        int projLoc = glGetUniformLocation(shader.ID, "projection");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        //Draw elements
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

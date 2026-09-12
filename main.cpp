#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "headers/shaderclass.h"
#include "headers/input.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iomanip>
#include <tinyexr.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    if (width == 0 || height == 0) return;
    glViewport(0, 0, width, height);
}

// CUBE VERTICES
float vertices[] = {
    // pos                  // normal
    // -Z
    -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,

    // +Z
    -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,

    // -X
    -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,

    // +X
     0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,

     // -Y
    -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,

    // +Y
    -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f
};

unsigned int indices[] = {
    // -Z (Back)
    0, 1, 2,        2, 3, 0,

    // +Z (Front)
    4, 5, 6,        6, 7, 4,

    // -X (Left)
    8, 9, 10,       10, 11, 8,

    // +X (Right)
    12, 13, 14,     14, 15, 12,

    // -Y (Bottom)
    16, 17, 18,     18, 19, 16,

    // +Y (Top)
    20, 21, 22,     22, 23, 20
};

// CAMERA
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

    if (firstMouse){
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


void coordinates_callback()
{
    if (cameraPos != lastPos) std::cout << "X: " << std::fixed << std::setprecision(2) << cameraPos.x << " ----- Y: " << std::fixed << std::setprecision(2) << cameraPos.y << " ----- Z: " << std::fixed << std::setprecision(2) << cameraPos.z << std::endl;

    lastPos = cameraPos;
}

int main()
{
    // GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Medieval RPG", nullptr, nullptr);

    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return -2;
    }

    glfwMakeContextCurrent(window);

    // GLAD
    if (!gladLoadGL()) {
        std::cerr << "Failed to load GLAD" << std::endl;
        glfwTerminate();
        return -3;
    }

    // OPENGL SETUP
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // LOAD HDRI
    const char* filename = R"(assets/NightSkyHDRI007_16K_HDR.exr)";
    float* rgba = nullptr;
    int width = 0;
    int height = 0;
    const char* err = nullptr;
    int ret = LoadEXR(&rgba, &width, &height, filename, &err);

    if (ret != TINYEXR_SUCCESS) {
        std::cerr << "Failed to load EXR: " << (err ? err : "unknown error") << std::endl;
        if (err) FreeEXRErrorMessage(err);

        glfwTerminate();
        return -4;
    }

    /*
    std::cout << "Loaded EXR successfully" << std::endl;
    std::cout << "Height: " << height << std::endl;
    std::cout << "Width: " << width << std::endl;
    */

    // HDR TEXTURE
    unsigned int hdrTextureID;
    glGenTextures(1, &hdrTextureID);
    glBindTexture(GL_TEXTURE_2D, hdrTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, rgba);

    // Equirectangular texture:
    // horizontal axis should repeat
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    free(rgba);
    rgba = nullptr;

    // CREATE CUBEMAP
    unsigned int envCubemap;
    glGenTextures(1, &envCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

    for (unsigned int i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 2048, 2048, 0, GL_RGB, GL_FLOAT, nullptr);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // CAPTURE FRAMEBUFFER
    unsigned int captureFBO, captureRBO;

    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 2048, 2048);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

    // CUBEMAP CAPTURE MATRICES
    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 captureViews[] = {
        // +X
        glm::lookAt(glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),

        // -X
        glm::lookAt(glm::vec3(0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),

        // +Y
        glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),

        // -Y
        glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),

        // +Z
        glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),

        // -Z
        glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))
    };

    // VAO / VBO / EBO
    GLuint vao, vbo, ebo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // SHADERS
    Shader shader("shaders/vertexshader.vert", "shaders/fragmentshader.frag");
    Shader hdrToCubemapShader("shaders/hdr_to_cubemap.vert", "shaders/hdr_to_cubemap.frag");
    Shader skyboxShader("shaders/skybox.vert", "shaders/skybox.frag");

    shader.use();
    shader.setVec3("lightDir", glm::normalize(glm::vec3(0.4f, 1.0f, 0.3f)));
    shader.setInt("skybox", 1);

    // HDR -> CUBEMAP SHADER
    hdrToCubemapShader.use();
    hdrToCubemapShader.setInt("equirectangularMap", 0);
    hdrToCubemapShader.setMat4("projection", captureProjection);

    // SKYBOX SHADER
    skyboxShader.use();
    skyboxShader.setInt("environmentMap", 0);

    // CONVERT EQUIRECTANGULAR -> CUBEMAP
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrTextureID);

    glViewport(0, 0, 2048, 2048);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glDisable(GL_DEPTH_TEST);
    hdrToCubemapShader.use();

    for (unsigned int i = 0; i < 6; i++) {
        hdrToCubemapShader.setMat4("view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
    }

    // RETURN TO WINDOW
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glEnable(GL_DEPTH_TEST);

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    glViewport(0, 0, screenWidth, screenHeight);

    // GAME VARIABLES
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float rotationAngle = 0.0f;

    Input input(window);

    // MAIN LOOP
    while (!glfwWindowShouldClose(window))
    {
        // TIME
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // INPUT
        input.windowInput(window, cameraPos, cameraFront, deltaTime);

        // ROTATE CUBE
        rotationAngle += 60.0f * deltaTime;

        // GET CURRENT WINDOW SIZE
        glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
        if (screenWidth == 0 || screenHeight == 0) {
            glfwPollEvents();
            continue;
        }
        glViewport(0, 0, screenWidth, screenHeight);

        // CAMERA MATRICES
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projection = glm::perspective(glm::radians(60.0f), static_cast<float>(screenWidth) / static_cast<float>(screenHeight), 0.1f, 1000.0f);

        // PROVISORY GROUND COLLISION
        if (cameraPos.y < 0.0f) cameraPos.y = 0.0f;

        // CLEAR SCREEN
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        // SKYBOX
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_FALSE);
        skyboxShader.use();
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);


        // CUBE
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

        shader.use();

        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        shader.setVec3("lightDir", glm::normalize(glm::vec3(0.4f, 1.0f, 0.3f)));

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
        shader.setInt("skybox", 1);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

        // DEBUG COORDINATES
        coordinates_callback();

        // SWAP
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // CLEANUP
    glDeleteTextures(1, &hdrTextureID);
    glDeleteTextures(1, &envCubemap);
    glDeleteRenderbuffers(1, &captureRBO);
    glDeleteFramebuffers(1, &captureFBO);
    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glfwTerminate();

    return 0;
}
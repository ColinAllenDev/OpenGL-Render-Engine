#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

/* Forward Declaration */
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

/* Settings */
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

/* Camera */
Camera camera(glm::vec3(1.0f, 1.0f, 1.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

/* Timing */
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
    glfwInit();

    /* GLFW Configuration */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* GLFW Window Initialization */
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "RenderEngine", NULL, NULL);
    if (window == NULL) {
        std::cerr << " Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowPos(window, 1650, 200); // Temporary

    /* Set Callbacks */
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    /* Capture Mouse */
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    /* GLAD Initialization */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    /* Configure global OpenGL State */
    glEnable(GL_DEPTH_TEST);

    /* Build and compile shader program */
    Shader cube_shader("/home/colin/Projects/RenderEngine/src/shaders/default_v.glsl",
                       "/home/colin/Projects/RenderEngine/src/shaders/default_f.glsl");
    Shader sphere_shader("/home/colin/Projects/RenderEngine/src/shaders/light/light_v.glsl",
                       "/home/colin/Projects/RenderEngine/src/shaders/light/light_f.glsl");


    /* Load Models */
    Model cube("/home/colin/Projects/RenderEngine/res/models/cube.obj");
    Model sphere("/home/colin/Projects/RenderEngine/res/models/sphere.obj");

    /* Wireframe Mode */
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    /* Render Loop */
    while(!glfwWindowShouldClose(window)) {
        /* Per-frame Time Logic */
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        /* Process Input */
        process_input(window);

        /* Rendering Operations */
        // Background
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // State-setting
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // State-using

        /** CUBE **/
        // Activate Shader
        cube_shader.Use();

        // Lighting
        cube_shader.SetVec3("light.position", glm::vec3(1.2f, 1.0f, 2.0f));
        cube_shader.SetVec3("viewPos", camera.Position);

        glm::vec3 lightColor;
        lightColor.x = sin(glfwGetTime() * 2.0f);
        lightColor.y = sin(glfwGetTime() * 0.7f);
        lightColor.z = sin(glfwGetTime() * 1.3f);
        glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);
        cube_shader.SetVec3("light.ambient", ambientColor);
        cube_shader.SetVec3("light.diffuse", diffuseColor);
        cube_shader.SetVec3("light.specular", 1.0f, 1.0f, 1.0f);

        cube_shader.SetVec3("material.ambient", 1.0f, 0.5f, 0.31f);
        cube_shader.SetVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
        cube_shader.SetVec3("material.specular", 0.5f, 0.5f, 0.5f); // specular lighting doesn't have full effect on this object's material
        cube_shader.SetFloat("material.shininess", 32.0f);

        // Pass projection matrix to shader
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_HEIGHT / (float)SCR_HEIGHT, 0.1f, 100.0f);
        cube_shader.SetMat4("projection", projection);

        // Camera/View Transformation
        glm::mat4 view = camera.GetViewMatrix();
        cube_shader.SetMat4("view", view);

        // Render Cube
        glm::mat4 cube_model = glm::mat4(1.0f);
        cube_model = glm::translate(cube_model, glm::vec3(0.0f, -1.0f, 0.0f));
        cube_model = glm::scale(cube_model, glm::vec3(2.0f, 0.25f, 2.0f));

        cube_shader.SetMat4("model", cube_model);

        cube.Draw(cube_shader);

        /** SPHERE **/
        sphere_shader.Use();

        sphere_shader.SetMat4("projection", projection);
        sphere_shader.SetMat4("view", view);

        glm::mat4 sphere_model = glm::mat4(1.0f);
        sphere_model = glm::translate(sphere_model, glm::vec3(1.2f, 1.0f, 2.0f));
        sphere_model = glm::scale(sphere_model, glm::vec3(0.25f, 0.25f, 0.25f));
        sphere_shader.SetMat4("model", sphere_model);

        sphere.Draw(sphere_shader);


        /* Swap buffers and poll events */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    /* Terminate GLFW */
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window) {
    /* Terminating the window */
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    /* Camera Controls */
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset, true);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
    camera.ProcessMouseScroll(yOffset);
}
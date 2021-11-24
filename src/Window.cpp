#include "Window.h"

Window::Window(const char* title, unsigned int width, unsigned int height, unsigned int posX, unsigned int posY) {
    this->title = title;
    this->width = width;
    this->height = height;

    /* GLFW Init */
    glfwInit();

    /* GLFW Configuration */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Initialize Window */
    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == NULL) {
        std::cerr << "ERROR::GLFW Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowPos(window, posX, posY);

    glfwSetFramebufferSizeCallback(window, FramebufferSize_callback);
};

void Window::FramebufferSize_callback(GLFWwindow* window, int width, int height) {

}
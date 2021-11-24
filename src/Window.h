#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <GLFW/glfw3.h>
#include <glad/glad.h>

class Window {
public:
    const char* title;
    unsigned int width, height;
    Window(const char* title, unsigned int width, unsigned int height);

    GLFWwindow GetWindow();
private:
    static void FramebufferSize_callback(GLFWwindow* window, int height, int width);
};

#endif
#include "Window.h"
#include "Engine.h"

void Window::InitializeGLFW() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    //glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
}

void Window::InitializeWindow() {
    m_GLFWwindow = glfwCreateWindow(1280, 720, "Vulkan Game", NULL, NULL);

    glfwSetFramebufferSizeCallback(m_GLFWwindow, resizeCallback);
}

GLFWwindow* Window::getGLFWwindow() {
    return m_GLFWwindow;
}

bool Window::ShouldWindowClose() {
    return glfwWindowShouldClose(m_GLFWwindow);
}

void Window::pollEvents() {
    glfwPollEvents();
}

void Window::Terminate() {
    glfwDestroyWindow(m_GLFWwindow);
    glfwTerminate();
}

void Window::resizeCallback(GLFWwindow* window, int width, int height) {
    Engine::m_Renderer.framebufferResized = true;
}
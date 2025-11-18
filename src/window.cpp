#include "window.hpp"

#include <stdexcept>

namespace Cosmos {

    Window::Window(int width, int height, const char* name) : WIDTH(width), HEIGHT(height), windowName(name) {
        initWindow();
    }

    
    void Window::initWindow()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        
        window = glfwCreateWindow(WIDTH, HEIGHT, windowName.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    }
    
    Window::~Window() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
    
    void Window::framebufferResizeCallback(GLFWwindow *window, int width, int height)
    {
        auto _window = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
        _window->framebufferResized = true;
        _window->WIDTH = width;
        _window->HEIGHT = height;
    }
    
    bool Window::shouldClose() {
        return glfwWindowShouldClose(window);
    }

    void Window::pollEvents() {
        glfwPollEvents();
    }

    void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
    {
        if(glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface!");
        }
    }
}
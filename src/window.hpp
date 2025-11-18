#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

namespace Cosmos {
    
    class Window
    {
    public:
        Window(int width, int height, const char* name);
        ~Window();
        // deleting copy constructor and assignment operator to prevent copying
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;  

        bool shouldClose();
        void pollEvents();
        
        void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
        
        bool wasWindowResized() {return framebufferResized;}
        void resetWindowResizedFlag() {framebufferResized = false;}


        VkExtent2D getExtent() {return {static_cast<uint32_t>(WIDTH), static_cast<uint32_t>(HEIGHT)};}

    private:
        static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
        void initWindow();
        

        int WIDTH = 800;
        int HEIGHT = 600;
        bool framebufferResized = false; // flag to see if window resized

        std::string windowName;
        GLFWwindow* window;
    };

}
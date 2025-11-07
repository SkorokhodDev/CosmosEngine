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

        VkExtent2D getExtent() {return {static_cast<uint32_t>(WIDTH), static_cast<uint32_t>(HEIGHT)};}

        private:
        void initWindow();

        const int WIDTH = 800;
        const int HEIGHT = 600;

        std::string windowName;
        GLFWwindow* window;
    };

}
#include "app.hpp"

namespace Cosmos {
    
    void Application::run() {
        // Main application loop goes here
         while (!window.shouldClose()) {
             window.pollEvents();
         }
    }

}
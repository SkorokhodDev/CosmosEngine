#pragma once

#include <vector>

#include "window.hpp"
#include "engine_device.hpp"
#include "game_object.hpp"
#include "renderer.hpp"
#include "simple_render_system.hpp"
#include "camera.hpp"

namespace Cosmos {

    class Application
    {
    public:
        static constexpr int WIDTH = 1200;
        static constexpr int HEIGHT = 800;

        Application();
        ~Application();

        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;  

        void run();

    private:
        void loadGameObjects();

        Window window{WIDTH, HEIGHT, "Cosmos Engine"};
        EngineDevice engineDevice{window};
        Renderer renderer{window, engineDevice};
        std::vector<GameObject> gameObjects;

    };

} 
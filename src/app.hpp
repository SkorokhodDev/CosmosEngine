#pragma once

#include <vector>

#include "window.hpp"
#include "engine_device.hpp"
#include "game_object.hpp"
#include "renderer.hpp"
#include "systems/simple_render_system.hpp"
#include "systems/point_light_system.hpp"
#include "camera.hpp"
#include "keyboard_movement_controller.hpp"
#include "descriptors.hpp"

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

        // note: order of declarations matters
        std::unique_ptr<DescriptorPool> globalPool{};
        GameObject::Map gameObjects;

    };

} 
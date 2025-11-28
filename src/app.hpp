#pragma once

#include <vector>

#include "window.hpp"
#include "pipeline.hpp"
#include "engine_swap_chain.hpp"
#include "engine_device.hpp"
#include "model.hpp"
#include "game_object.hpp"

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
        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void freeCommandBuffers();
        void drawFrame();
        void loadGameObjects();
        void recreateSwapChain();
        void recordCommandBuffer(int imageIndex);
        void renderGameObjects(VkCommandBuffer commandBuffer);

        Window window{WIDTH, HEIGHT, "Cosmos Engine"};
        EngineDevice engineDevice{window};
        std::unique_ptr<EngineSwapChain> engineSwapChain;
        std::unique_ptr<Pipeline> ptr_Pipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
        std::vector<GameObject> gameObjects;

    };

} 
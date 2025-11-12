#pragma once

#include <vector>

#include "window.hpp"
#include "pipeline.hpp"
#include "engine_swap_chain.hpp"
#include "engine_device.hpp"

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
        void drawFrame();

        Window window{WIDTH, HEIGHT, "Cosmos Engine"};
        EngineDevice engineDevice{window};
        EngineSwapChain engineSwapChain{engineDevice, window.getExtent()};
        std::unique_ptr<Pipeline> ptr_Pipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
    };

} 
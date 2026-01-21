#pragma once

#include <vector>

#include "pipeline.hpp"
#include "engine_device.hpp"
#include "game_object.hpp"
#include "camera.hpp"
#include "frame_info.hpp"

namespace Cosmos {

    class SimpleRenderSystem
    {
    public:
        SimpleRenderSystem(EngineDevice& device, VkRenderPass renderPass);
        ~SimpleRenderSystem();

        SimpleRenderSystem(const SimpleRenderSystem&) = delete;
        SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;  

        void run();

        void renderGameObjects(FrameInfo& frameInfo, std::vector<GameObject>& gameObjects);
    
    private:
        void createPipelineLayout();
        void createPipeline(VkRenderPass renderPass);

        EngineDevice& engineDevice;
        std::unique_ptr<Pipeline> ptr_Pipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<GameObject> gameObjects;

    };

} 
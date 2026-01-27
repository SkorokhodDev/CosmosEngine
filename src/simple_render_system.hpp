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
        SimpleRenderSystem(EngineDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
        ~SimpleRenderSystem();

        SimpleRenderSystem(const SimpleRenderSystem&) = delete;
        SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;  

        void run();

        void renderGameObjects(FrameInfo& frameInfo);
    
    private:
        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void createPipeline(VkRenderPass renderPass);

        EngineDevice& engineDevice;
        std::unique_ptr<Pipeline> ptr_Pipeline;
        VkPipelineLayout pipelineLayout;

    };

} 
#pragma once

#include <vector>

#include "pipeline.hpp"
#include "engine_device.hpp"
#include "game_object.hpp"
#include "camera.hpp"
#include "frame_info.hpp"

namespace Cosmos {

    class PointLightSystem
    {
    public:
        PointLightSystem(EngineDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
        ~PointLightSystem();

        PointLightSystem(const PointLightSystem&) = delete;
        PointLightSystem& operator=(const PointLightSystem&) = delete;  

        void run();

        void update(FrameInfo& frameInfo, GlobalUbo& ubo);
        void render(FrameInfo& frameInfo);
    
    private:
        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void createPipeline(VkRenderPass renderPass);

        EngineDevice& engineDevice;
        std::unique_ptr<Pipeline> ptr_Pipeline;
        VkPipelineLayout pipelineLayout;

    };

} 
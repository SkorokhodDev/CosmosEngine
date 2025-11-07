#include "app.hpp"

#include <stdexcept>

namespace Cosmos {

    Application::Application()
    {
        createPipelineLayout();
        createPipeline();
        createCommandBuffers();
    }

    Application::~Application()
    {
        vkDestroyPipelineLayout(engineDevice.device(), pipelineLayout, nullptr);
    }

    void Application::run() {
        // Main application loop goes here
         while (!window.shouldClose()) {
             window.pollEvents();
         }
    }

    void Application::createPipelineLayout()
    {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;
        if(vkCreatePipelineLayout(engineDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }
    void Application::createPipeline()
    {
        auto pipelineConfig = Pipeline::defaultPipelineConfigInfo(engineSwapChain.width(), engineSwapChain.height());
        pipelineConfig.renderPass = engineSwapChain.getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        ptr_Pipeline = std::make_unique<Pipeline>(engineDevice, 
            "../shaders/simple_shader.vert.spv",
            "../shaders/simple_shader.frag.spv", 
            pipelineConfig);
    }
    void Application::createCommandBuffers()
    {
    }
    void Application::drawFrame()
    {
    }
}
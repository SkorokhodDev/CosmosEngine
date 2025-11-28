#include "app.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>

#include <iostream>

namespace Cosmos {

    struct SimplePushConstantData{
        glm::mat2 transform{1.f}; // diagonal == 1, all others is 0
        glm::vec2 offset;
        alignas(16) glm::vec3 color;
    };

    Application::Application()
    {
        // firsly load models
        loadGameObjects();

        createPipelineLayout();
        recreateSwapChain();
        createCommandBuffers();
    }

    Application::~Application()
    {
        vkDestroyPipelineLayout(engineDevice.device(), pipelineLayout, nullptr);
    }


    void Application::run() {
        // Main application loop goes here
        while (!window.shouldClose()) 
        {
            window.pollEvents();
            drawFrame();
        }
        // Fixes validation layer erros after closing app
        vkDeviceWaitIdle(engineDevice.device());
    }

    void Application::createPipelineLayout()
    {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstantData);

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
        if(vkCreatePipelineLayout(engineDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }
    void Application::createPipeline()
    {
        assert(engineSwapChain != nullptr && "Cannot create pipeline before swap chain");
        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

        PipelineConfigInfo pipelineConfig{};
        Pipeline::defaultPipelineConfigInfo(pipelineConfig);

        pipelineConfig.renderPass = engineSwapChain->getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        ptr_Pipeline = std::make_unique<Pipeline>(
            engineDevice, 
            "../shaders/simple_shader.vert.spv",
            "../shaders/simple_shader.frag.spv", 
            pipelineConfig);
    }

    // Allocates command buffer
    void Application::createCommandBuffers()
    {
        commandBuffers.resize(engineSwapChain->imageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = engineDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
        
        if(vkAllocateCommandBuffers(engineDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate command buffers!");
        }
    }

    void Application::freeCommandBuffers()
    {
        vkFreeCommandBuffers(
            engineDevice.device(),
            engineDevice.getCommandPool(),
            static_cast<uint32_t>(commandBuffers.size()), 
            commandBuffers.data()
        );
        commandBuffers.clear();
    }

    void Application::loadGameObjects()
    {
        std::vector<Model::Vertex> vertices{
            {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
        };

        //better use shared ptr, bec it allows us to use single model between multiple game objects
        auto ptr_Model = std::make_shared<Model>(engineDevice, vertices);

        auto triangle = GameObject::createGameObject();
        triangle.model = ptr_Model;
        triangle.color = {0.1f, 0.8f, 0.1f};
        triangle.transform2d.translation.x = .2f;
        triangle.transform2d.scale = {2.f, 0.5f};
        triangle.transform2d.rotation = 0.25 * glm::two_pi<float>();

        gameObjects.push_back(std::move(triangle));
    }

    void Application::recreateSwapChain()
    {
        auto _extent = window.getExtent();
        while(_extent.width == 0 || _extent.height == 0)
        {
            glfwWaitEvents();
            _extent = window.getExtent();
        }
        vkDeviceWaitIdle(engineDevice.device());

        //engineSwapChain.reset(); // can fix VK_ERROR_OUT_OF_HOST_MEMORY
        // Fixing memory management 
        if(engineSwapChain == nullptr) 
        {
            engineSwapChain = std::make_unique<EngineSwapChain>(engineDevice, _extent);
        }
        else
        {
            engineSwapChain = std::make_unique<EngineSwapChain>(engineDevice, _extent, std::move(engineSwapChain));
            if(engineSwapChain->imageCount() != commandBuffers.size()) 
            {
                freeCommandBuffers();
                createCommandBuffers();
            }
        }

        createPipeline();
    }

    void Application::drawFrame()
    {
        uint32_t imageIndex;
        auto result = engineSwapChain->acquireNextImage(&imageIndex);

        // surface has changed in such a way that it is no longer compatible with the swapchain
        if (result == VK_ERROR_OUT_OF_DATE_KHR) 
        {
            recreateSwapChain();
            return;
        }

        if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        recordCommandBuffer(imageIndex);
        result  = engineSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.wasWindowResized())
        {
            window.resetWindowResizedFlag();
            recreateSwapChain();
            return;
        }

        if(result != VK_SUCCESS)
        {
            throw std::runtime_error("failed to present swap chain image!");
        }
    }

    void Application::recordCommandBuffer(int imageIndex)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if(vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to begin recording command buffer");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = engineSwapChain->getRenderPass();
        renderPassInfo.framebuffer = engineSwapChain->getFrameBuffer(imageIndex);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = engineSwapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0}; // 0, not 0.0f
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();
        
        vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(engineSwapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(engineSwapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{{0,0}, engineSwapChain->getSwapChainExtent()};
        vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
        vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

        renderGameObjects(commandBuffers[imageIndex]);

        vkCmdEndRenderPass(commandBuffers[imageIndex]);

        if(vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to record command buffer!");
        }
    }

    void Application::renderGameObjects(VkCommandBuffer commandBuffer)
    {
        ptr_Pipeline->bind(commandBuffer);
        
        for(auto& obj : gameObjects)
        {
            obj.transform2d.rotation = glm::mod(obj.transform2d.rotation + 0.01f, glm::two_pi<float>());

            SimplePushConstantData push{};
            push.offset = obj.transform2d.translation;
            push.color = obj.color;
            //std::cout << push.color.x << " " << push.color.y << std::endl;
            push.transform = obj.transform2d.mat2();

            vkCmdPushConstants(commandBuffer, 
                pipelineLayout, 
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                sizeof(SimplePushConstantData),
                &push);
            
            obj.model->bind(commandBuffer);
            obj.model->draw(commandBuffer);
        }
    }
}
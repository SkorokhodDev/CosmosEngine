#include "renderer.hpp"

#include <stdexcept>
#include <array>

#include <iostream>

namespace Cosmos {

    Renderer::Renderer(Window& window, EngineDevice& device) : window{window}, engineDevice{device}
    {
        recreateSwapChain();
        createCommandBuffers();
    }

    Renderer::~Renderer() 
    {
        freeCommandBuffers();       
    }

    VkCommandBuffer Renderer::beginFrame()
    {
        assert(!isFrameStarted && "Cant call beginFrame while already in progress");
        auto result = engineSwapChain->acquireNextImage(&currentImageIndex);
        // surface has changed in such a way that it is no longer compatible with the swapchain
        if (result == VK_ERROR_OUT_OF_DATE_KHR) 
        {
            recreateSwapChain();
            return nullptr;
        }

        if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        isFrameStarted = true;
        auto commandBuffer = getCurrentCommandBuffer();
        
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if(vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to begin recording command buffer");
        }
        return commandBuffer;
    }

    void Renderer::endFrame()
    {
        assert(isFrameStarted && "Renderer -> endFrame(): Cant call endFrame while frame is not in progress");
        auto commandBuffer = getCurrentCommandBuffer();

        if(vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to record command buffer!");
        }

        auto result  = engineSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.wasWindowResized())
        {
            window.resetWindowResizedFlag();
            recreateSwapChain();
        }
        else if(result != VK_SUCCESS)
        {
            throw std::runtime_error("failed to present swap chain image!");
        }
        isFrameStarted = false;
        currentFrameIndex = (currentFrameIndex + 1) % EngineSwapChain::MAX_FRAMES_IN_FLIGHT;
    }

    void Renderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer)
    {
        assert(isFrameStarted && "Cant call beginSwapChainRenderPass while already in progress");
        assert(commandBuffer == getCurrentCommandBuffer() 
        && "Cant begin render pass on command buffer from a different frame");

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = engineSwapChain->getRenderPass();
        renderPassInfo.framebuffer = engineSwapChain->getFrameBuffer(currentImageIndex);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = engineSwapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0}; // 0, not 0.0f
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();
        
        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(engineSwapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(engineSwapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{{0,0}, engineSwapChain->getSwapChainExtent()};
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    }

    void Renderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer)
    {
        assert(isFrameStarted && "Cant call endSwapChainRenderPass while already in progress");
        assert(commandBuffer == getCurrentCommandBuffer() 
        && "Cant end render pass on command buffer from a different frame");

        vkCmdEndRenderPass(commandBuffer);
    }

    // Allocates command buffer
    void Renderer::createCommandBuffers()
    {
        commandBuffers.resize(EngineSwapChain::MAX_FRAMES_IN_FLIGHT);

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

    void Renderer::freeCommandBuffers()
    {
        vkFreeCommandBuffers(
            engineDevice.device(),
            engineDevice.getCommandPool(),
            static_cast<uint32_t>(commandBuffers.size()), 
            commandBuffers.data()
        );
        commandBuffers.clear();
    }

    void Renderer::recreateSwapChain()
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
            std::shared_ptr<EngineSwapChain> oldSwapChain = std::move(engineSwapChain);
            engineSwapChain = std::make_unique<EngineSwapChain>(engineDevice, _extent, oldSwapChain);
            if(!oldSwapChain->compareSwapFormats(*engineSwapChain.get()))
            {
                throw std::runtime_error("Swap chain image (or depth) format has changed!");
            }
        }

    }
}
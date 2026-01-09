#pragma once

#include <vector>
#include <memory>
#include <cassert>

#include "window.hpp"
#include "engine_swap_chain.hpp"
#include "engine_device.hpp"
#include "model.hpp"

namespace Cosmos {

    class Renderer
    {
    public:

        Renderer(Window& window, EngineDevice& device);
        ~Renderer();

        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;  

        VkCommandBuffer beginFrame();
        void endFrame();
        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
        void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

        VkRenderPass getSwapChainRenderPass() const {return engineSwapChain->getRenderPass(); }
        float getAspectRatio() const {return engineSwapChain->extentAspectRatio();}
        bool isFrameInProgress() const {return isFrameStarted;}

        VkCommandBuffer getCurrentCommandBuffer() const {
            assert(isFrameStarted && "Renderer -> isFrameInProgress(): Cannot get command buffer when frame not in progress");
            return commandBuffers[currentFrameIndex];
        }

        int getFrameIndex() const {
            assert(isFrameStarted && "Cant call beginSwapChainRenderPass while already in progress");
            return currentFrameIndex;
        }
        
    private:
        void createCommandBuffers();
        void freeCommandBuffers();
        void recreateSwapChain();

        Window& window;
        EngineDevice& engineDevice;
        std::unique_ptr<EngineSwapChain> engineSwapChain;
        std::vector<VkCommandBuffer> commandBuffers;

        uint32_t currentImageIndex;
        int currentFrameIndex = 0;
        bool isFrameStarted = false;
    };

} 
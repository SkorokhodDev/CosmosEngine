#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>

#include "engine_device.hpp"

namespace Cosmos {

    class Model {
    public:

    // Note: when update Vertex, dont forget to update Atttribute Description
        struct Vertex {
            glm::vec3 position;
            glm::vec3 color;

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
        };

        Model(EngineDevice &device, const std::vector<Vertex> &vertices);
        ~Model();

        Model(const Model&) = delete;
        Model& operator=(const Model&) = delete;  

        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer); 

    private:
        EngineDevice& engineDevice;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        uint32_t vertexCount;

        void createVertexBuffer(const std::vector<Vertex> &vertices);

    };
}
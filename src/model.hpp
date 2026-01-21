#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>
#include <memory>

#include "engine_device.hpp"
#include "buffer.hpp"

namespace Cosmos {

    class Model {
    public:

    // Note: when update Vertex, dont forget to update Atttribute Description
        struct Vertex {
            glm::vec3 position;
            glm::vec3 color;
            glm::vec3 normal{};
            glm::vec2 uv{};

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
       
            bool operator==(const Vertex& other) const {
                return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
            }
        };

        struct Builder {
            std::vector<Vertex> vertices{};
            std::vector<uint32_t> indices{};
            
            void loadModel(const std::string &filepath);
        };

        Model(EngineDevice &device, const Model::Builder &builder);
        ~Model();

        Model(const Model&) = delete;
        Model& operator=(const Model&) = delete;  

        static std::unique_ptr<Model> createModelFromFile(EngineDevice& device, const std::string& filepath);

        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer); 

    private:
        void createVertexBuffers(const std::vector<Vertex> &vertices);
        void createIndexBuffer(const std::vector<uint32_t> &indices);

        EngineDevice& engineDevice;

        std::unique_ptr<Buffer> vertexBuffer;
        uint32_t vertexCount;
        
        bool hasIndexBuffer = false;
        std::unique_ptr<Buffer> indexBuffer;
        uint32_t indexCount;
    };
}
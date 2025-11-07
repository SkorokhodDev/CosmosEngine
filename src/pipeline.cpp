#include "pipeline.hpp"

#include <fstream>
#include <stdexcept>
#include <iostream>

namespace Cosmos {

    Pipeline::Pipeline(MyEngineDevice& device, const std::string& vertFilePath, const std::string& fragFilePath,
        const PipelineConfigInfo& configInfo) : engineDevice{device} 
    {
        createGraphicsPipeline(vertFilePath, fragFilePath, configInfo);
    }

    Pipeline::~Pipeline() {
        // Destructor implementation
    }

    PipelineConfigInfo Pipeline::defaultPipelineConfigInfo(uint32_t width, uint32_t height)
    {
        PipelineConfigInfo configInfo{};
        return configInfo;
    }


    std::vector<char> Pipeline::readFile(const std::string& filePath) {
        // ate -> when file is opened, seeked the end immediately
        // binary -> read file as binary
        std::ifstream file(filePath, std::ios::ate | std::ios::binary);
        if(!file.is_open()) {
            throw std::runtime_error("failed to open file: " + filePath);
        }

        size_t fileSize = static_cast<size_t>(file.tellg()); //tellg -> last position
        
        std::vector<char> buffer(fileSize);

        file.seekg(0); //go back to beginning
        file.read(buffer.data(), fileSize);
        file.close();
        return buffer;
    }
    
    void Pipeline::createGraphicsPipeline(const std::string& vertPath, const std::string& fragPath,  const PipelineConfigInfo& configInfo) {
        auto vertCode = readFile(vertPath);
        auto fragCode = readFile(fragPath);

        std::cout << "Vertex Shader Code Size: " << vertCode.size() << " bytes\n";
        std::cout << "Fragment Shader Code Size: " << fragCode.size() << " bytes\n";
    }

    void Pipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule)
    {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data()); // pointer to code data

        if (vkCreateShaderModule(engineDevice.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shader module!");
        }
    }

} // namespace Cosmos
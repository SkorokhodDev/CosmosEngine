#pragma once

#include <string>
#include <vector>

#include "my_engine_device.hpp"

namespace Cosmos {

    struct PipelineConfigInfo {};


    class Pipeline {
    public:
        Pipeline(MyEngineDevice& device, const std::string& vertFilePath, const std::string& fragFilePath,
                 const PipelineConfigInfo& configInfo);
        ~Pipeline();
        
        Pipeline(const Pipeline&) = delete;
        void operator=(const Pipeline&) = delete;

        static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

        private:
        static std::vector<char> readFile(const std::string& filePath);

        void createGraphicsPipeline(const std::string& vertPath, const std::string& fragPath,
            const PipelineConfigInfo& configInfo);
        
        void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule); 

        MyEngineDevice& engineDevice;
        VkPipeline graphicsPipeline;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;
    };

} // namespace Cosmos
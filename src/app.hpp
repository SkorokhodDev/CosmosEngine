#pragma once

#include "window.hpp"
#include "pipeline.hpp"
#include "my_engine_device.hpp"

namespace Cosmos {

    class Application
    {
        public:
        static constexpr int WIDTH = 1200;
        static constexpr int HEIGHT = 800;

        void run();
        private:
        Window window{WIDTH, HEIGHT, "Cosmos Engine"};
        MyEngineDevice engineDevice{window};
        // read compiled code, not source code!!!
        Pipeline m_pipeline{
            engineDevice, 
            "../shaders/simple_shader.vert.spv", 
            "../shaders/simple_shader.frag.spv", 
            Pipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)
        };
        
    };

} 
#pragma once

#include "window.hpp"

namespace Cosmos {

    class Application
    {
        public:
        static constexpr int WIDTH = 1200;
        static constexpr int HEIGHT = 800;

        void run();
        private:
        Window window{WIDTH, HEIGHT, "Cosmos Engine"};
    };

}
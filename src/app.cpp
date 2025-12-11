#include "app.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>

#include <iostream>

namespace Cosmos {


    Application::Application()
    {
        // firsly load models
        loadGameObjects();
    }

    Application::~Application()
    {
    }


    void Application::run() {

        SimpleRenderSystem simpleRenderSystem{engineDevice, renderer.getSwapChainRenderPass()};
        // Main application loop goes here
        while (!window.shouldClose()) 
        {
            window.pollEvents();

            if(auto commandBuffer = renderer.beginFrame())
            {
                renderer.beginSwapChainRenderPass(commandBuffer);
                
                simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
                
                renderer.endSwapChainRenderPass(commandBuffer);
                renderer.endFrame();
            }
        }
        // Fixes validation layer erros after closing app
        vkDeviceWaitIdle(engineDevice.device());
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
    
}
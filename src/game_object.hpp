#pragma once

#include <memory>

#include <model.hpp>

namespace Cosmos {

    struct Transform2dComponent {
        glm::vec2 translation{};
        glm::vec2 scale{1.f, 1.f};
        float rotation;

        glm::mat2 mat2() {
            const float s = glm::sin(rotation);
            const float c = glm::cos(rotation);
            glm::mat2 rotMatrix{{c, s}, {-s, c}};

            glm::mat2 scaleMatrix{{scale.x, 0.0f},{0.0f, scale.y}}; // colums
            return rotMatrix * scaleMatrix; 
        }

    };
    
    class GameObject{
        
    public:
        using id_t = unsigned int;

        GameObject(const GameObject&) = delete;
        GameObject &operator =(const GameObject&) = delete;
        GameObject(GameObject&&) = default;
        GameObject& operator = (GameObject&&) = default;

        static GameObject createGameObject() 
        {
            static id_t currentId = 0;
            return GameObject{currentId++};
        }

        id_t getId() const {return id;}

        std::shared_ptr<Model> model{};
        glm::vec3 color{};
        Transform2dComponent transform2d;

    private:
        GameObject(id_t objId) : id(objId) {};
        
        id_t id;
    };
}
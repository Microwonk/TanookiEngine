#pragma once

#include "tnk_model.hpp"

#include <memory>

namespace tnk {

    struct Transform2DComponent {
        glm::vec2 translation{}; // position offset
        glm::vec2 scale{1.f, 1.f};
        float rotation;

        glm::mat2 mat2() {
            const float s = glm::sin(rotation);
            const float c = glm::cos(rotation);
            glm::mat2 rotMat{{c, s}, {-s, c}};

            glm::mat2 scaleMat{{scale.x, .0f}, {.0f, scale.y}};
            return rotMat * scaleMat;
        }
    };

    class TnkGameObject {
    public:
        using id_t = unsigned int;

        static TnkGameObject createGameObject() {
            static id_t currentId = 0;
            return TnkGameObject{currentId++};
        }

        TnkGameObject(const TnkGameObject &) = delete;
        TnkGameObject &operator=(const TnkGameObject &) = delete;
        // move constructors
        TnkGameObject(TnkGameObject&&) = default;
        TnkGameObject &operator=(TnkGameObject&&) = default;

        id_t getId() { return id; }

        std::shared_ptr<TnkModel> model{};
        glm::vec3 color {};
        Transform2DComponent transform2d;

    private:
        TnkGameObject(id_t objId) : id{objId} {}

        id_t id;
    };

}
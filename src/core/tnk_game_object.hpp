#pragma once

#include "graphics/model.hpp"
#include "transform_component.hpp"

#include <unordered_map>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>

namespace tnk {

    // TODO implement ECS with
    // https://austinmorlan.com/posts/entity_component_system/
    class TnkGameObject {
    public:
        using id_t = unsigned int;
        using Map = std::unordered_map<id_t, TnkGameObject>;

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
        TransformComponent transform{};

    private:
        TnkGameObject(id_t objId) : id{objId} {}

        id_t id;
    };

}
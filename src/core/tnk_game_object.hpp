#pragma once

#include "graphics/model.hpp"


#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>

namespace tnk {

    struct TransformComponent {
        glm::vec3 translation{}; // position offset
        glm::vec3 scale{1.f, 1.f, 1.f};
        glm::quat orientation{};

        glm::mat4 mat4() {
            // Convert translation to a mat4
            glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), translation);
            return translationMat * glm::mat4_cast(orientation);
        }

        glm::mat3 normalMatrix() {
            const glm::mat3 rotationMat = glm::mat3_cast(orientation);
            const glm::mat3 invScaleMat = glm::mat3(1.0f / scale.x, 0.0f, 0.0f,
                                                    0.0f, 1.0f / scale.y, 0.0f,
                                                    0.0f, 0.0f, 1.0f / scale.z);
            return glm::transpose(glm::inverse(rotationMat * invScaleMat));
        }

        void addRotation(const float pitch, const float yaw, const float roll) {
            glm::quat qPitch = glm::angleAxis(pitch, glm::vec3{1, 0, 0});
            glm::quat qYaw = glm::angleAxis(yaw, glm::vec3{0, 1, 0});
            glm::quat qRoll = glm::angleAxis(roll,glm::vec3{0,0,1});

            // Combine qPitch and qYaw in a single step
            orientation = glm::normalize(qPitch * orientation * qYaw * qRoll);
        }

        void addRotation(const float pitch, const float yaw) {
            glm::quat qPitch = glm::angleAxis(pitch, glm::vec3{1, 0, 0});
            glm::quat qYaw = glm::angleAxis(yaw, glm::vec3{0, 1, 0});

            // VERY IMPORTANT TO MULTIPLY : pitch, then orientation, then yaw!
            orientation = glm::normalize(qPitch * orientation * qYaw);
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
        TransformComponent transform{};

    private:
        TnkGameObject(id_t objId) : id{objId} {}

        id_t id;
    };

}
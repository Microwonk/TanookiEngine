#pragma once

#include "tnk_model.hpp"


#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>

namespace tnk {

    struct TransformComponent {
        glm::vec3 translation{}; // position offset
        glm::vec3 scale{1.f, 1.f, 1.f};
//        glm::quat rotation{};
        glm::vec3 rotation{};

        glm::mat4 mat4() {
            const float c3 = glm::cos(rotation.z);
            const float s3 = glm::sin(rotation.z);
            const float c2 = glm::cos(rotation.x);
            const float s2 = glm::sin(rotation.x);
            const float c1 = glm::cos(rotation.y);
            const float s1 = glm::sin(rotation.y);
            return glm::mat4{{scale.x * (c1 * c3 + s1 * s2 * s3),
                              scale.x * (c2 * s3),
                              scale.x * (c1 * s2 * s3 - c3 * s1),
                              0.0f},
                             {scale.y * (c3 * s1 * s2 - c1 * s3),
                              scale.y * (c2 * c3),
                              scale.y * (c1 * c3 * s2 + s1 * s3),
                              0.0f},
                              {scale.z * (c2 * s1),
                               scale.z * (-s2),
                               scale.z * (c1 * c2),
                               0.0f},
                               {translation.x, translation.y, translation.z, 1.0f}};
        }

        glm::mat3 normalMatrix() {
            const float c3 = glm::cos(rotation.z);
            const float s3 = glm::sin(rotation.z);
            const float c2 = glm::cos(rotation.x);
            const float s2 = glm::sin(rotation.x);
            const float c1 = glm::cos(rotation.y);
            const float s1 = glm::sin(rotation.y);
            const glm::vec3 invScale = 1.0f / scale;
            return glm::mat3{
                {
                    invScale.x * (c1 * c3 + s1 * s2 * s3),
                    invScale.x * (c2 * s3),
                    invScale.x * (c1 * s2 * s3 - c3 * s1),
                },
                {
                    invScale.y * (c3 * s1 * s2 - c1 * s3),
                    invScale.y * (c2 * c3),
                    invScale.y * (c1 * c3 * s2 + s1 * s3),
                },
                {
                    invScale.z * (c2 * s1),
                    invScale.z * (-s2),
                    invScale.z * (c1 * c2),
                },
            };
        }

        // TODO later quats
//        void setRotation(const glm::vec3& eulerAngles) {
//            rotation = glm::quat(eulerAngles);
//        }
//
//        void setRotation(const glm::quat& quat) {
//            rotation = quat;
//        }
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
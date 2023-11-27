#pragma once

#include "glm/vec3.hpp"
#include "glm/gtc/quaternion.hpp"

namespace tnk {

    struct TransformComponent {
        glm::vec3 translation{};
        glm::vec3 scale{1.f, 1.f, 1.f};
        glm::quat orientation{};

        glm::mat4 mat4() {
            // Convert translation and scale to a mat4
            glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), translation);
            glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), scale);
            return translationMat * scaleMat * glm::mat4_cast(orientation);
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

        void addRotation(glm::vec3 pitchYawRoll) {
            addRotation(pitchYawRoll.x, pitchYawRoll.y, pitchYawRoll.z);
        }
    };
}
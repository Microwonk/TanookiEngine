#pragma once

#include "../tnk_game_object.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE // openGL uses -1 to 0, this uses 0 to 1
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include <cassert>
#include <limits>

namespace tnk {

    class TnkCamera {
    public:
        void setOrthoProjection(float left, float right, float top, float bottom, float near, float far);
        void setPerspectiveProjection(float fov, float aspect, float near, float far);

        const glm::mat4& getProjectionMatrix() const { return projectionMatrix; }
        glm::mat4& getViewMatrix();

        TransformComponent transform{};

    private:
        glm::mat4 projectionMatrix{1.f};
        glm::mat4 viewMatrix{1.f};
    };


}

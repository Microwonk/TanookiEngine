#include "camera.hpp"

namespace tnk {
    void TnkCamera::setOrthoProjection(float left, float right, float top, float bottom, float near, float far) {
        projectionMatrix = glm::mat4{1.0f};
        projectionMatrix[0][0] = 2.f / (right - left);
        projectionMatrix[1][1] = 2.f / (bottom - top);
        projectionMatrix[2][2] = 1.f / (far - near);
        projectionMatrix[3][0] = -(right + left) / (right - left);
        projectionMatrix[3][1] = -(bottom + top) / (bottom - top);
        projectionMatrix[3][2] = -near / (far - near);
    }

    void TnkCamera::setPerspectiveProjection(float fov, float aspect, float near, float far) {
        assert(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
        const float tanHalfFov = tan(fov / 2.f);
        projectionMatrix = glm::mat4{0.0f};
        projectionMatrix[0][0] = 1.f / (aspect * tanHalfFov);
        projectionMatrix[1][1] = 1.f / (tanHalfFov);
        projectionMatrix[2][2] = far / (far - near);
        projectionMatrix[2][3] = 1.f;
        projectionMatrix[3][2] = -(far * near) / (far - near);
    }

    glm::mat4& TnkCamera::getViewMatrix() {
        transform.orientation = glm::normalize(transform.orientation);
        glm::mat4 rotate = glm::mat4_cast(transform.orientation);

        glm::mat4 translate = glm::mat4(1.0f);
        translate = glm::translate(translate, -transform.translation);

        viewMatrix = rotate * translate;
        return viewMatrix;
    }
}
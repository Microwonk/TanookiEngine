#include "camera.hpp"

namespace tnk {
    void TnkCamera::setOrthoProjection(float left, float right, float top, float bottom, float near, float far) {
        // is just some setup maths that has to be done this way for the projection to work like this
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
        // is just some setup maths that has to be done this way for the projection to work like this
        const float tanHalfFov = tan(fov / 2.f);
        projectionMatrix = glm::mat4{0.0f};
        projectionMatrix[0][0] = 1.f / (aspect * tanHalfFov);
        projectionMatrix[1][1] = 1.f / (tanHalfFov);
        projectionMatrix[2][2] = far / (far - near);
        projectionMatrix[2][3] = 1.f;
        projectionMatrix[3][2] = -(far * near) / (far - near);
    }

    glm::mat4& TnkCamera::getViewMatrix() {
        // first normalize the quaternion to get unit length
        transform.orientation = glm::normalize(transform.orientation);
        // cast the quaternion to a 4x4 matrix to set viewMatrix
        glm::mat4 rotate = glm::mat4_cast(transform.orientation);

        // real translation is the 4x4 identity matrix - the vec3 of the translation x y z components
        glm::mat4 translate = glm::mat4(1.0f);
        translate = glm::translate(translate, -transform.translation);

        // the viewMatrix is computed by multiplying the rotation and the translation matrices in this order (non-commutative)
        viewMatrix = rotate * translate;
        return viewMatrix;
    }

    // TODO look into why i need a -eyeMat.x ??
    glm::vec3 TnkCamera::eye() {
        // the direction vector the camera is looking at (the eye)                            this vector is for going along the z-axis
        auto eyeVec = glm::mat3(glm::mat4_cast(transform.orientation)) * glm::vec3{0, 0, 1.0f};
        return glm::vec3{-eyeVec.x, -eyeVec.y, eyeVec.z};
    }
}
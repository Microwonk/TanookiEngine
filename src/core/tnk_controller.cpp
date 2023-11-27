#include "tnk_controller.hpp"

namespace tnk {

    // TODO quaternions generally use for rotations

    void TnkController::moveInPlaneXZ(GLFWwindow *window, float deltaTime, TnkCamera &camera) {

        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) moveSpeed = 6.f;
        else moveSpeed = 3.f;

        if (glfwGetKey(window, keys.close) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

        // Handle mouse movement
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        // Calculate the change in mouse position
        double deltaX = mouseX - lastMouseX;
        double deltaY = mouseY - lastMouseY;
        // Update the last mouse position for the next frame
        lastMouseX = mouseX;
        lastMouseY = mouseY;

        // calculate the difference between last mouse pos and current mouse pos
        // and multiply it by delta time for consistent speed
        glm::vec3 rotate{0};
        // might be confusing because the x and y are flipped, but that is because the vector is in order of pitch, yaw, roll.
        rotate.x += static_cast<float>(deltaY);
        rotate.y -= static_cast<float>(deltaX);
        rotate *= deltaTime;
        // rotate of z should always be 0. no roll needed for this camera

        camera.transform.addRotation(rotate);

        auto eye = camera.eye();

        const glm::vec3 forwardDir{eye.x, 0, eye.z}; // putting eye.y in here will make it possible to move in the y plane without shift and space
        const glm::vec3 rightDir{eye.z, 0, -eye.x};
        const glm::vec3 upDir{0.f, 1.f, 0.f};

        glm::vec3 moveDirection{0.f};
        if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS) moveDirection += forwardDir;
        if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) moveDirection -= forwardDir;

        if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS) moveDirection += rightDir;
        if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) moveDirection -= rightDir;

        if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS) moveDirection -= upDir;
        if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS) moveDirection += upDir;

        // the vectors must not be 0, this is how we check
        if (glm::dot(moveDirection, moveDirection) > std::numeric_limits<float>::epsilon()){
            camera.transform.translation += moveSpeed * deltaTime * glm::normalize(moveDirection);
        }

    }
}
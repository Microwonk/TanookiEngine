#include "tnk_controller.hpp"

namespace tnk {

    // TODO quaternions generally use for rotations

    void TnkController::moveInPlaneXZ(GLFWwindow *window, float deltaTime, TnkCamera &camera) {

        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) moveSpeed = 40.f;
        else moveSpeed = 20.f;

        if (glfwGetKey(window, keys.close) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

        glm::vec3 rotate{0};

        // Handle mouse movement
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        // Calculate the change in mouse position
        double deltaX = mouseX - lastMouseX;
        double deltaY = mouseY - lastMouseY;
        // Update the last mouse position for the next frame
        lastMouseX = mouseX;
        lastMouseY = mouseY;

        rotate.x -= static_cast<float>(deltaX) * deltaTime;
        rotate.y -= static_cast<float>(deltaY) * deltaTime;

        camera.transform.addRotation(-rotate.y, rotate.x);

        auto lookDir = glm::mat3(glm::mat4_cast(camera.transform.orientation)) * glm::vec3{0, 0, 1.0f};

        const glm::vec3 forwardDirection{-lookDir.x, 0.0, lookDir.z};
        const glm::vec3 rightDirection{forwardDirection.z, 0.f, -forwardDirection.x};
        const glm::vec3 upDirection{0.f, 1.f, 0.f};

        glm::vec3 moveDirection{0.f};

        if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS) moveDirection += forwardDirection;
        if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) moveDirection -= forwardDirection;

        if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS) moveDirection += rightDirection;
        if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) moveDirection -= rightDirection;

        if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS) moveDirection -= upDirection;
        if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS) moveDirection += upDirection;

        // the vectors must not be 0, this is how we check
        if (glm::dot(moveDirection, moveDirection) > std::numeric_limits<float>::epsilon()){
            camera.transform.translation += moveSpeed * deltaTime * glm::normalize(moveDirection);
        }

    }
}
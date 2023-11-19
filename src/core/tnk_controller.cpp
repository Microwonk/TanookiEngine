#include "tnk_controller.hpp"

namespace tnk {
    void TnkController::moveInPlaneXZ(GLFWwindow *window, float deltaTime, tnk::TnkGameObject &gameObject) {

        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) moveSpeed = 5.f;
        else moveSpeed = 3.f;


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

        // Adjust the camera orientation based on mouse movement
        const float sensitivity = 0.1f;
        deltaX *= sensitivity;
        deltaY *= sensitivity;

        rotate.y += static_cast<float>(deltaX);
        rotate.x -= static_cast<float>(deltaY);

        if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
            gameObject.transform.rotation += lookSpeed * deltaTime * glm::normalize(rotate);
        }

        gameObject.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
        gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());

        float yaw = gameObject.transform.rotation.y;
        const glm::vec3 forwardDirection{sin(yaw), 0.f, cos(yaw)};
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
            gameObject.transform.translation += moveSpeed * deltaTime * glm::normalize(moveDirection);
        }

    }
}
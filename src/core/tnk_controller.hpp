#pragma once

#include "graphics/camera.hpp"
#include "tnk_window.hpp"

namespace tnk {
    class TnkController {
    public:
        struct KeyMappings {
            int moveLeft = GLFW_KEY_A;
            int moveRight = GLFW_KEY_D;
            int moveForward = GLFW_KEY_W;
            int moveBackward = GLFW_KEY_S;
            int moveUp = GLFW_KEY_SPACE;
            int moveDown = GLFW_KEY_LEFT_SHIFT;
            int close = GLFW_KEY_ESCAPE;
        };

        void moveInPlaneXZ(GLFWwindow* window, float deltaTime, TnkCamera& camera);

        KeyMappings keys{};
        float moveSpeed{};
        float lastMouseX{0.f};
        float lastMouseY{0.f};
    };
}
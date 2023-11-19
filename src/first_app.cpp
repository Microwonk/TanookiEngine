#include "first_app.hpp"

namespace tnk {

    FirstApp::FirstApp() { loadGameObjects(); }

    FirstApp::~FirstApp() {}

    void FirstApp::run() {
        SimpleRenderSystem simpleRenderSystem{tnkDevice, tnkRenderer.getSwapChainRenderPass()};
        TnkCamera camera{};
        camera.setViewTarget(glm::vec3(-1.f, -2.f, 10.f), glm::vec3(0.f, 0.f, 2.5f));

        auto viewerObject = TnkGameObject::createGameObject();
        TnkController cameraController{};

        auto startTime = std::chrono::high_resolution_clock::now();
        auto lastTime = startTime;
        uint32_t frameCount = 0;

        // capture cursor
        glfwSetInputMode(tnkWindow.getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        tnkRenderer.setClearColor(glm::vec3(0.1f, 0.1f, 0.1f));

        while (!tnkWindow.shouldClose()) {
            glfwPollEvents();

            // Calculate frame time
            auto currentTime = std::chrono::high_resolution_clock::now();
            auto frameTime = std::chrono::duration<float>(currentTime - lastTime).count();
            lastTime = currentTime;

            // Update camera and other logic
            cameraController.moveInPlaneXZ(tnkWindow.getGLFWwindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            float aspect = tnkRenderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(70.f), aspect, 0.1f, 100.f);

            // Render the frame
            if (auto commandBuffer = tnkRenderer.beginFrame()) {
                tnkRenderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
                tnkRenderer.endSwapChainRenderPass(commandBuffer);
                tnkRenderer.endFrame();
            }

            frameCount++;

            // Update FPS every second
            auto elapsedSeconds = std::chrono::duration<float>(currentTime - startTime).count();
            if (elapsedSeconds >= 1.0f) {
                tnkFps = static_cast<float>(frameCount) / elapsedSeconds;
                frameCount = 0;
                startTime = currentTime;
                std::cout << "[FPS]> " << tnkFps << " | [Frame time]> " << frameTime << '\n';
            }
        }

        vkDeviceWaitIdle(tnkDevice.device());
    }

    void FirstApp::loadGameObjects() {
        std::shared_ptr<TnkModel> tnkModel = TnkModel::createModelFromFile(tnkDevice, "models/smooth_vase.obj");

        auto cube = TnkGameObject::createGameObject();
        cube.model = tnkModel;
        cube.transform.translation = {.0f, .0f, 0.f};
        cube.transform.scale = glm::vec3{3.f};

        gameObjects.push_back(std::move(cube));
    }

    float FirstApp::fps() { return tnkFps; }
}

#include "first_app.hpp"

namespace tnk {

    struct GlobalUbo {
        glm::mat4 projectionView{1.f};
        glm::vec3 lightDirection = glm::normalize(glm::vec3{1.f, -3.f, -1.f});
    };

    FirstApp::FirstApp() { loadGameObjects(); }

    FirstApp::~FirstApp() {}

    void FirstApp::run() {

        std::vector<std::unique_ptr<TnkBuffer>> uboBuffers(TnkSwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < uboBuffers.size(); ++i) {
            uboBuffers[i] = std::make_unique<TnkBuffer>(
                    tnkDevice,
                    sizeof(GlobalUbo),
                    1,
                    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
                    tnkDevice.properties.limits.minUniformBufferOffsetAlignment
                    );
            uboBuffers[i]->map();
        }

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
            camera.setPerspectiveProjection(glm::radians(70.f), aspect, 0.1f, 1000.f);

            // Render the frame
            if (auto commandBuffer = tnkRenderer.beginFrame()) {
                int frameIndex = tnkRenderer.getFrameIndex();
                FrameInfo frameInfo {
                    frameIndex,
                    frameTime,
                    commandBuffer,
                    camera,
                };

                // update
                GlobalUbo ubo{};
                ubo.projectionView = camera.getProjectionMatrix() * camera.getViewMatrix();
                uboBuffers[frameIndex]->writeToBuffer(&ubo);
                uboBuffers[frameIndex]->flush();

                // render
                tnkRenderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(frameInfo, gameObjects);
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
                std::cout << "[FPS]> " << fps() << " | [Frame time]> " << frameTime << '\n';
            }
        }

        vkDeviceWaitIdle(tnkDevice.device());
    }

    void FirstApp::loadGameObjects() {
        std::shared_ptr<TnkModel> model = TnkModel::createModelFromFile(tnkDevice, "models/colored_cube.obj");

        const float cubeSize = 1.f;
        const float spacing = 10.f;
        const int cubesPerSide = 5;
        const float cubeStartPosition = -cubesPerSide * (cubeSize + spacing) / 2.0f;

        for (int x = 0; x < cubesPerSide; ++x) {
            for (int y = 0; y < cubesPerSide; ++y) {
                for (int z = 0; z < cubesPerSide; ++z) {
                    auto cube = TnkGameObject::createGameObject();
                    cube.model = model;
                    cube.transform.translation = {
                            cubeStartPosition + x * (cubeSize + spacing),
                            cubeStartPosition + y * (cubeSize + spacing),
                            cubeStartPosition + z * (cubeSize + spacing)
                    };
                    cube.transform.scale = glm::vec3{cubeSize};
                    gameObjects.push_back(std::move(cube));
                }
            }
        }
    }

    float FirstApp::fps() { return tnkFps; }
}

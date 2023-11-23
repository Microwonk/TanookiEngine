#include "first_app.hpp"

namespace tnk {

    struct GlobalUbo {
        glm::mat4 projectionView{1.f};
        glm::vec3 lightDirection = glm::normalize(glm::vec3{1.f, -3.f, -1.f});
    };

    FirstApp::FirstApp() {

        // pool with max set of 2 (Max frames in flight), addPoolSize can add any buffer (UNIFORM, STORAGE etc.)
        globalPool = TnkDescriptorPool::Builder(tnkDevice)
                .setMaxSets(TnkSwapChain::MAX_FRAMES_IN_FLIGHT)
                .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, TnkSwapChain::MAX_FRAMES_IN_FLIGHT)
                .build();
        loadGameObjects();
    }

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

        auto globalSetLayout = TnkDescriptorSetLayout::Builder(tnkDevice)
                .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
                .build();

        std::vector<VkDescriptorSet> globalDescriptorSets(TnkSwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < globalDescriptorSets.size(); ++i) {
            auto bufferInfo = uboBuffers[i]->descriptorInfo();
            TnkDescriptorWriter(*globalSetLayout, *globalPool)
                .writeBuffer(0, &bufferInfo)
                .build(globalDescriptorSets[i]);
        }

        SimpleRenderSystem simpleRenderSystem{tnkDevice, tnkRenderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};
        TnkCamera camera{};

        auto viewerObject = TnkGameObject::createGameObject();
        TnkController cameraController{};

        auto startTime = std::chrono::high_resolution_clock::now();
        auto lastTime = startTime;
        uint32_t frameCount = 0;

        // capture cursor
        glfwSetInputMode(tnkWindow.getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        tnkRenderer.setClearColor(glm::vec3(0.1f, 0.1f, 0.1f));

        float rot = 0.001f;

        while (!tnkWindow.shouldClose()) {
            glfwPollEvents();

            // Calculate frame time
            auto currentTime = std::chrono::high_resolution_clock::now();
            auto frameTime = std::chrono::duration<float>(currentTime - lastTime).count();
            lastTime = currentTime;

            // Update camera and other logic
            cameraController.moveInPlaneXZ(tnkWindow.getGLFWwindow(), frameTime, camera);

            float aspect = tnkRenderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(60.f), aspect, 0.1f, 1000.f);

            for (auto& obj : gameObjects) {
                obj.transform.addRotation(rot, rot, rot);
            }

            // Render the frame
            if (auto commandBuffer = tnkRenderer.beginFrame()) {
                int frameIndex = tnkRenderer.getFrameIndex();
                FrameInfo frameInfo {
                    frameIndex,
                    frameTime,
                    commandBuffer,
                    camera,
                    globalDescriptorSets[frameIndex]
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
        const int cubesPerSide = 20;
        const float cubeStartPosition = -cubesPerSide * (cubeSize + spacing) / 2.0f;

        for (int x = 0; x < cubesPerSide; ++x) {
            for (int y = 0; y < cubesPerSide; ++y) {
                for (int z = 0; z < cubesPerSide; ++z) {
                    auto cube = TnkGameObject::createGameObject();
                    cube.model = model;
                    cube.transform.translation = {
                            cubeStartPosition + x * (cubeSize + spacing) + rand() / 100,
                            cubeStartPosition + y * (cubeSize + spacing) + rand() / 100,
                            cubeStartPosition + z * (cubeSize + spacing) + rand() / 100,
                    };
                    cube.transform.scale = glm::vec3{cubeSize};
                    gameObjects.push_back(std::move(cube));
                }
            }
        }
    }

    float FirstApp::fps() { return tnkFps; }
}

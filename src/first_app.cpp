#include "first_app.hpp"

namespace tnk {

    struct GlobalUbo {
        glm::mat4 projection{1.f};
        glm::mat4 view{1.f};
        glm::vec4 ambientColor{1.f, 1.f, 1.f, .02f}; // w is intensity
        glm::vec3 lightPosition{-1.f};
        alignas(16) glm::vec4 lightColor{0.5f, 0.5f, 1.f, 1.f}; // w is light intensity
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
                .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS) // either ALL_GRAPHICS or VERTEX | FRAGMENT
                .build();

        std::vector<VkDescriptorSet> globalDescriptorSets(TnkSwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < globalDescriptorSets.size(); ++i) {
            auto bufferInfo = uboBuffers[i]->descriptorInfo();
            TnkDescriptorWriter(*globalSetLayout, *globalPool)
                .writeBuffer(0, &bufferInfo)
                .build(globalDescriptorSets[i]);
        }

        SimpleRenderSystem simpleRenderSystem{tnkDevice, tnkRenderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};
        PointLightSystem pointLightSystem{tnkDevice, tnkRenderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};
        TnkCamera camera{};
        camera.transform.translation.z = -3.0f;

        TnkController cameraController{};

        auto startTime = std::chrono::high_resolution_clock::now();
        auto lastTime = startTime;
        uint32_t frameCount = 0;

        // capture cursor
        glfwSetInputMode(tnkWindow.getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        tnkRenderer.setClearColor(glm::vec3(0.3f, 0.6f, 1));


        while (!tnkWindow.shouldClose()) {
            glfwPollEvents();

            auto currentTime = std::chrono::high_resolution_clock::now();
            auto frameTime = std::chrono::duration<float>(currentTime - lastTime).count();
            lastTime = currentTime;

            // Update camera and other logic
            cameraController.moveInPlaneXZ(tnkWindow.getGLFWwindow(), frameTime, camera);

            float aspect = tnkRenderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(60.f), aspect, 0.1f, 1000.f);

            // Render the frame
            if (auto commandBuffer = tnkRenderer.beginFrame()) {
                int frameIndex = tnkRenderer.getFrameIndex();
                FrameInfo frameInfo {
                    frameIndex,
                    frameTime,
                    commandBuffer,
                    camera,
                    globalDescriptorSets[frameIndex],
                    gameObjects
                };

                // update
                GlobalUbo ubo{};
                ubo.projection = camera.getProjectionMatrix();
                ubo.view = camera.getViewMatrix();
                uboBuffers[frameIndex]->writeToBuffer(&ubo);
                uboBuffers[frameIndex]->flush();

                // render
                tnkRenderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(frameInfo);
                pointLightSystem.render(frameInfo);
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
        std::shared_ptr<TnkModel> model = TnkModel::createModelFromFile(tnkDevice, "models/plants.obj");

        auto vase = TnkGameObject::createGameObject();
        vase.model = model;
        vase.transform.translation = glm::vec3{.5f, .5f, 0};
        vase.transform.scale = glm::vec3{0.1f, 0.1f, 0.1f};
        gameObjects.emplace(vase.getId(), std::move(vase));

        model = TnkModel::createModelFromFile(tnkDevice, "models/quad.obj");

        auto floor = TnkGameObject::createGameObject();
        floor.model = model;
        floor.transform.translation = glm::vec3{0, .5f, 0};
        floor.transform.scale = glm::vec3{3.f};
        gameObjects.emplace(floor.getId(), std::move(floor));
    }

    float FirstApp::fps() { return tnkFps; }
}

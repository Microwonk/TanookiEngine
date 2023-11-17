#include "first_app.hpp"

namespace tnk {

    FirstApp::FirstApp() { loadGameObjects(); }

    FirstApp::~FirstApp() {}

    void FirstApp::run() {

        SimpleRenderSystem simpleRenderSystem{tnkDevice, tnkRenderer.getSwapChainRenderPass()};

        auto lastTime = std::chrono::high_resolution_clock::now();
        uint32_t frameCount = 0;

        while (!tnkWindow.shouldClose()) {
            glfwPollEvents();

            if (auto commandBuffer = tnkRenderer.beginFrame()) {
                tnkRenderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
                tnkRenderer.endSwapChainRenderPass(commandBuffer);
                tnkRenderer.endFrame();
            }

            // Calculate FPS
            frameCount++;
            auto currentTime = std::chrono::high_resolution_clock::now();
            auto deltaTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - lastTime).count();

            if (deltaTime >= 1.0) {
                tnkFps = static_cast<double>(frameCount) / deltaTime;
                frameCount = 0;
                lastTime = currentTime;
                std::cout << "[FPS]> " << fps() << '\n';
            }
        }

        vkDeviceWaitIdle(tnkDevice.device());
    }

    void FirstApp::loadGameObjects() {
        std::vector<TnkModel::Vertex> vertices{
                {{-0.5f, 0.5f}, {}},
                {{0.5f, 0.5f}, {}},
                {{0.0f, -0.5f}, {}}
        };

        auto tnkModel = std::make_shared<TnkModel>(tnkDevice, vertices);

        auto sp = TnkGameObject::createGameObject();
        sp.model = tnkModel;
        sp.color = {.1f, .0f, .1f};
        sp.transform2d.translation.x = .2f;

        gameObjects.push_back(std::move(sp));
    }

    float FirstApp::fps() { return tnkFps; }
}

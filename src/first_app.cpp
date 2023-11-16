#include "first_app.hpp"

namespace tnk {

    FirstApp::FirstApp() {
        createPipelineLayout();
        createPipeline();
        createCommandBuffers();
    }

    FirstApp::~FirstApp() {
        vkDestroyPipelineLayout(tnkDevice.device(), pipelineLayout, nullptr);
    }

	void FirstApp::run() {
        auto lastTime = std::chrono::high_resolution_clock::now();
        uint32_t frameCount = 0;

        while (!tnkWindow.shouldClose()) {
            glfwPollEvents();
            drawFrame();

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

    float FirstApp::fps() { return tnkFps; }

    void FirstApp::createPipelineLayout() {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;

        if (vkCreatePipelineLayout(tnkDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout.");
        }
    }

    void FirstApp::createPipeline() {
        auto pipelineConfig = TnkPipeline::defaultPipelineConfigInfo(tnkSwapChain.width(), tnkSwapChain.height());
        pipelineConfig.renderPass = tnkSwapChain.getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        tnkPipeline = std::make_unique<TnkPipeline>(
                tnkDevice,
                pipelineConfig,
                "simple_shader.vert.spv",
                "simple_shader.frag.spv");
    }

    void FirstApp::createCommandBuffers() {
        commandBuffers.resize(tnkSwapChain.imageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = tnkDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(tnkDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers.");
        }

        for (int i = 0; i < commandBuffers.size(); ++i) {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
                throw std::runtime_error("failed to begin recording command buffer!");
            }

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = tnkSwapChain.getRenderPass();
            renderPassInfo.framebuffer = tnkSwapChain.getFrameBuffer(i);

            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = tnkSwapChain.getSwapChainExtent();

            std::array<VkClearValue, 2> clearValues {};
            clearValues[0].color = {{0.1, 0.1, 0.1, 0.1}};
            clearValues[1].depthStencil = {1.0, 0};
            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            tnkPipeline->bind(commandBuffers[i]);
            vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

            vkCmdEndRenderPass(commandBuffers[i]);

            if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to record command buffer.");
            }
        }

    }

    void FirstApp::drawFrame() {
        uint32_t imageIndex;
        auto result = tnkSwapChain.acquireNextImage(&imageIndex);

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image.");
        }

        result = tnkSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image.");
        }
    }
}

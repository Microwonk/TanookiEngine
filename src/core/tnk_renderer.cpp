#include "tnk_renderer.hpp"

namespace tnk {

    TnkRenderer::TnkRenderer(TnkWindow &window, TnkDevice &device) : tnkWindow{window}, tnkDevice{device} {
        recreateSwapChain();
        createCommandBuffers();
    }

    TnkRenderer::~TnkRenderer() { freeCommandBuffers(); }

    void TnkRenderer::recreateSwapChain() {
        auto extent = tnkWindow.getExtent();
        while (extent.width == 0 || extent.height == 0) {
            extent = tnkWindow.getExtent();
            glfwWaitEvents();
        }
        vkDeviceWaitIdle(tnkDevice.device());

        if (tnkSwapChain == nullptr) {
            tnkSwapChain = std::make_unique<TnkSwapChain>(tnkDevice, extent);
        } else {
            std::shared_ptr<TnkSwapChain> oldSwapChain = std::move(tnkSwapChain);
            tnkSwapChain = std::make_unique<TnkSwapChain>(tnkDevice, extent, oldSwapChain);

            if (!oldSwapChain->compareSwapFormats(*tnkSwapChain.get())) {
                throw std::runtime_error("Swap chain image(or depth) format has changed");
            }
        }
    }

    void TnkRenderer::createCommandBuffers() {
        commandBuffers.resize(TnkSwapChain::MAX_FRAMES_IN_FLIGHT);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = tnkDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(tnkDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers.");
        }
    }

    void TnkRenderer::freeCommandBuffers() {
        vkFreeCommandBuffers(tnkDevice.device(),
                             tnkDevice.getCommandPool(),
                             static_cast<float>(commandBuffers.size()),
                             commandBuffers.data());
        commandBuffers.clear();
    }

    VkCommandBuffer TnkRenderer::beginFrame() {
        assert(!isFrameStarted && "Cannot call beginFrame when frame is in progress");

        auto result = tnkSwapChain->acquireNextImage(&currentImageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return nullptr;
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image.");
        }

        isFrameStarted = true;

        auto commandBuffer = getCurrentCommandBuffer();
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }
        return commandBuffer;
    }

    void TnkRenderer::endFrame() {
        assert(isFrameStarted && "Cannot call endFrame while frame is not in progress");
        auto commandBuffer = getCurrentCommandBuffer();
        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }

        auto result = tnkSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || tnkWindow.wasWindowResized()) {
            tnkWindow.resetWindowResizedFlag();
            recreateSwapChain();
        } else if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image.");
        }

        isFrameStarted = false;
        currentFrameIndex = (currentFrameIndex + 1) % TnkSwapChain::MAX_FRAMES_IN_FLIGHT;
    }

    void TnkRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer) {
        assert(isFrameStarted && "Cannot call beginSwapChainRenderPass while frame is not in progress");
        assert(commandBuffer == getCurrentCommandBuffer() && "Cannot begin render pass on command buffer from a different frame");

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = tnkSwapChain->getRenderPass();
        renderPassInfo.framebuffer = tnkSwapChain->getFrameBuffer(currentImageIndex);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = tnkSwapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {{0.1f, 0.1f, 0.1f, 1.0f}};
        clearValues[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(tnkSwapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(tnkSwapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{{0, 0}, tnkSwapChain->getSwapChainExtent()};
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    }

    void TnkRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer) {
        assert(isFrameStarted && "Cannot call endSwapChainRenderPass while frame is not in progress");
        assert(commandBuffer == getCurrentCommandBuffer() && "Cannot begin render pass on command buffer from a different frame");

        vkCmdEndRenderPass(commandBuffer);
    }
}

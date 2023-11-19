#pragma once

#include "tnk_window.hpp"
#include "tnk_device.hpp"
#include "tnk_swap_chain.hpp"
#include "tnk_model.hpp"

#include <memory>
#include <vector>

namespace tnk
{
    class TnkRenderer
    {
    public:

        TnkRenderer(TnkWindow &tnkWindow, TnkDevice &tnkDevice);
        ~TnkRenderer();

        VkRenderPass getSwapChainRenderPass() const { return tnkSwapChain->getRenderPass(); }
        float getAspectRatio() const { return tnkSwapChain->extentAspectRatio(); }
        bool isFrameInProgress() const { return isFrameStarted; }

        VkCommandBuffer getCurrentCommandBuffer() const {
            assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
            return commandBuffers[currentFrameIndex];
        }

        int getFrameIndex() const {
            assert(isFrameStarted && "Cannot get frame index when frame not in progress");
            return currentFrameIndex;
        }

        TnkRenderer(const TnkRenderer &) = delete;
        TnkRenderer &operator=(const TnkRenderer &) = delete;

        VkCommandBuffer beginFrame();
        void endFrame();
        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
        void endSwapChainRenderPass(VkCommandBuffer commandBuffer);
        void setClearColor(glm::vec3 color) { clearColor = color; }

    private:
        void createCommandBuffers();
        void freeCommandBuffers();
        void recreateSwapChain();

        TnkWindow &tnkWindow;
        TnkDevice &tnkDevice;
        std::unique_ptr<TnkSwapChain> tnkSwapChain;
        std::vector<VkCommandBuffer> commandBuffers;

        uint32_t currentImageIndex;
        int currentFrameIndex{0};
        bool isFrameStarted{false};
        glm::vec3 clearColor{};
    };
}
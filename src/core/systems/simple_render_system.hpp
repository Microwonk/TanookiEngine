#pragma once

#include "../graphics/pipeline.hpp"
#include "../graphics/device.hpp"
#include "../graphics/model.hpp"
#include "../tnk_game_object.hpp"
#include "../graphics/camera.hpp"
#include "../graphics/frame_info.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE // openGL uses -1 to 0, this uses 0 to 1
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

#include <memory>
#include <vector>

namespace tnk
{
    class SimpleRenderSystem
    {
    public:
        SimpleRenderSystem(TnkDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
        ~SimpleRenderSystem();

        SimpleRenderSystem(const SimpleRenderSystem &) = delete;
        SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;

        void renderGameObjects(FrameInfo &frameInfo);

    private:
        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void createPipeline(VkRenderPass renderPass);

        TnkDevice &tnkDevice;

        std::unique_ptr<TnkPipeline> tnkPipeline;
        VkPipelineLayout pipelineLayout;
    };
}
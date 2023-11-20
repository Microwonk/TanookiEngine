#pragma once

#include "tnk_pipeline.hpp"
#include "tnk_device.hpp"
#include "tnk_model.hpp"
#include "tnk_game_object.hpp"
#include "tnk_camera.hpp"
#include "tnk_frame_info.hpp"

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
        SimpleRenderSystem(TnkDevice &device, VkRenderPass renderPass);
        ~SimpleRenderSystem();

        SimpleRenderSystem(const SimpleRenderSystem &) = delete;
        SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;

        void renderGameObjects(FrameInfo &frameInfo, std::vector<TnkGameObject> &gameObjects);

    private:
        void createPipelineLayout();
        void createPipeline(VkRenderPass renderPass);

        TnkDevice &tnkDevice;

        std::unique_ptr<TnkPipeline> tnkPipeline;
        VkPipelineLayout pipelineLayout;
    };
}
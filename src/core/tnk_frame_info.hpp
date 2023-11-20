#pragma once

#include "tnk_camera.hpp"

#include <vulkan/vulkan.h>

namespace tnk {
    struct FrameInfo {
        int frameIndex;
        float frameTime;
        VkCommandBuffer commandBuffer;
        TnkCamera &camera;
    };
}
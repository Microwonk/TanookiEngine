#pragma once

#include "camera.hpp"

#include <vulkan/vulkan.h>

namespace tnk {
    struct FrameInfo {
        int frameIndex;
        float frameTime;
        VkCommandBuffer commandBuffer;
        TnkCamera &camera;
        VkDescriptorSet globalDescriptorSet;
    };
}
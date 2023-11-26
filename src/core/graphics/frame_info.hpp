#pragma once

#include "camera.hpp"
#include "../tnk_game_object.hpp"

#include <vulkan/vulkan.h>

namespace tnk {
    struct FrameInfo {
        int frameIndex;
        float frameTime;
        VkCommandBuffer commandBuffer;
        TnkCamera &camera;
        VkDescriptorSet globalDescriptorSet;
        TnkGameObject::Map &gameObjects;
    };
}
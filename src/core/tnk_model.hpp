#pragma once

#include "tnk_device.hpp"
#include "tnk_constants.hpp"
#include "tnk_utils.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE // openGL uses -1 to 0, this uses 0 to 1
#include "glm/glm.hpp"

#define GLM_ENABLE_EXPERIMENTAL // for hashing
#include <glm/gtx/hash.hpp>

#include <memory>
#include <cassert>

namespace tnk {
    class TnkModel {
    public:

        struct Vertex {
            glm::vec3 position{};
            glm::vec3 color{};
            glm::vec3 normal{};
            glm::vec2 uv{};

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

            bool operator==(const Vertex &other) const {
                return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
            }
        };

        struct Builder {
            std::vector<Vertex> vertices{};
            std::vector<uint32_t> indices{};

            void loadModel(const std::string &filepath);
        };

        TnkModel(TnkDevice &device, const TnkModel::Builder &builder, bool staging = false);
        ~TnkModel();

        TnkModel(const TnkModel &) = delete;
        TnkModel &operator=(const TnkModel &) = delete;

        static std::unique_ptr<TnkModel> createModelFromFile(TnkDevice &device, const std::string &filepath);

        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);

    private:
        void createVertexBuffers(const std::vector<Vertex> &vertices);
        void createIndexBuffers(const std::vector<uint32_t> &indices);
        void createVertexAndStagingBuffers(const std::vector<Vertex> &vertices);
        void createIndexAndStagingBuffers(const std::vector<uint32_t> &indices);

        TnkDevice& tnkDevice;

        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        uint32_t vertexCount;

        bool hasIndexBuffer{false};
        VkBuffer indexBuffer;
        VkDeviceMemory indexBufferMemory;
        uint32_t indexCount;
    };
}
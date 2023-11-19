#include "tnk_model.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace std {
    template <>
    struct hash<tnk::TnkModel::Vertex> {
        size_t operator()(tnk::TnkModel::Vertex const &vertex) const {
            size_t seed = 0;
            tnk::hashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
            return seed;
        }
    };
}

namespace tnk {

    TnkModel::TnkModel(tnk::TnkDevice &device, const TnkModel::Builder &builder, bool staging) : tnkDevice{device} {
        if (staging) {
            createVertexBuffers(builder.vertices);
            createIndexBuffers(builder.indices);
        } else {
            createVertexAndStagingBuffers(builder.vertices);
            createIndexAndStagingBuffers(builder.indices);
        }
    }

    TnkModel::~TnkModel() {
        vkDestroyBuffer(tnkDevice.device(), vertexBuffer, nullptr);
        vkFreeMemory(tnkDevice.device(), vertexBufferMemory, nullptr);

        if (hasIndexBuffer) {
            vkDestroyBuffer(tnkDevice.device(), indexBuffer, nullptr);
            vkFreeMemory(tnkDevice.device(), indexBufferMemory, nullptr);
        }
    }

    std::unique_ptr<TnkModel> TnkModel::createModelFromFile(TnkDevice &device, const std::string &filepath) {
        Builder builder{};
        builder.loadModel(filepath);

        return std::make_unique<TnkModel>(device, builder);
    }

    void TnkModel::createVertexBuffers(const std::vector<Vertex> &vertices) {
        vertexCount = static_cast<uint32_t>(vertices.size());
        assert(vertexCount >= 3 && "Vertex count must be at least three.");
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
        tnkDevice.createBuffer(bufferSize,
                               VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                               vertexBuffer,
                               vertexBufferMemory);

        void *data;
        vkMapMemory(tnkDevice.device(), vertexBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
        vkUnmapMemory(tnkDevice.device(), vertexBufferMemory);
    }

    // note that creating Staging-buffers only works best for performance if the host (CPU) does not change the data of the vertices too often
    //  , because if we do so, the extra copying operations from the buffer to the staging buffer may negate all performance increase we would gain
    // for example when making minecraft, where the mesh changes periodically, it is not best practice
    void TnkModel::createVertexAndStagingBuffers(const std::vector<Vertex> &vertices) {
        vertexCount = static_cast<uint32_t>(vertices.size());
        assert(vertexCount >= 3 && "Vertex count must be at least three.");
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        tnkDevice.createBuffer(bufferSize,
                               VK_BUFFER_USAGE_TRANSFER_SRC_BIT, // is going to be used for the memory transfer operation
                               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                               stagingBuffer,
                               stagingBufferMemory);

        void *data;
        vkMapMemory(tnkDevice.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
        vkUnmapMemory(tnkDevice.device(), stagingBufferMemory);

        tnkDevice.createBuffer(bufferSize,
                               VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, // transfer destination bit
                               VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                               vertexBuffer,
                               vertexBufferMemory);

        tnkDevice.copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

        // clean up the staging buffers
        vkDestroyBuffer(tnkDevice.device(), stagingBuffer, nullptr);
        vkFreeMemory(tnkDevice.device(), stagingBufferMemory, nullptr);
    }

    void TnkModel::createIndexBuffers(const std::vector<uint32_t> &indices) {
        indexCount = static_cast<uint32_t>(indices.size());
        hasIndexBuffer = indexCount > 0;

        if (!hasIndexBuffer) return;

        VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;
        tnkDevice.createBuffer(bufferSize,
                               VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                               indexBuffer,
                               indexBufferMemory);

        void *data;
        vkMapMemory(tnkDevice.device(), indexBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, indices.data(), static_cast<size_t>(bufferSize));
        vkUnmapMemory(tnkDevice.device(), indexBufferMemory);
    }

    void TnkModel::createIndexAndStagingBuffers(const std::vector<uint32_t> &indices) {
        indexCount = static_cast<uint32_t>(indices.size());
        hasIndexBuffer = indexCount > 0;

        if (!hasIndexBuffer) return;

        VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        tnkDevice.createBuffer(bufferSize,
                               VK_BUFFER_USAGE_TRANSFER_SRC_BIT, // is going to be used for the memory transfer operation
                               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                               stagingBuffer,
                               stagingBufferMemory);

        void *data;
        vkMapMemory(tnkDevice.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, indices.data(), static_cast<size_t>(bufferSize));
        vkUnmapMemory(tnkDevice.device(), stagingBufferMemory);

        tnkDevice.createBuffer(bufferSize,
                               VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, // transfer destination bit
                               VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                               indexBuffer,
                               indexBufferMemory);

        tnkDevice.copyBuffer(stagingBuffer, indexBuffer, bufferSize);

        // clean up the staging buffers
        vkDestroyBuffer(tnkDevice.device(), stagingBuffer, nullptr);
        vkFreeMemory(tnkDevice.device(), stagingBufferMemory, nullptr);
    }

    void TnkModel::draw(VkCommandBuffer commandBuffer) {
        // only draw indexed when indices are available
        if (hasIndexBuffer) {
            vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
        } else {
            vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
        }
    }

    void TnkModel::bind(VkCommandBuffer commandBuffer) {
        VkBuffer buffers[] = {vertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

        if (hasIndexBuffer) {
            // less complex models may need to adjust for only having VK_INDEX_TYPE_UINT16, to save memory
            vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
        }
    }

    std::vector<VkVertexInputBindingDescription> TnkModel::Vertex::getBindingDescriptions() {
        // could also be done like this:
        // return {{0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX}};
        std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
        bindingDescriptions[0].binding = 0;
        bindingDescriptions[0].stride = sizeof(Vertex);
        bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescriptions;
    }

    std::vector<VkVertexInputAttributeDescription> TnkModel::Vertex::getAttributeDescriptions() {
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
        attributeDescriptions[0].location = 0; // shader location
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, position);

        attributeDescriptions[1].location = 1; // shader location
        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);
        return attributeDescriptions;
    }

    void TnkModel::Builder::loadModel(const std::string &filepath) {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, (assetsDir + filepath).c_str())) {
            throw std::runtime_error(warn + err);
        }

        vertices.clear();
        indices.clear();

        std::unordered_map<Vertex, uint32_t> uniqueVertices{}; // for hashing uniques
        for (const auto &shape : shapes) {
            for (const auto &index : shape.mesh.indices) {
                Vertex vertex{};

                if (index.vertex_index >= 0) {
                    vertex.position = {
                            attrib.vertices[3 * index.vertex_index + 0],
                            attrib.vertices[3 * index.vertex_index + 1],
                            attrib.vertices[3 * index.vertex_index + 2],
                    };

                    int colorIndex = 3 * index.vertex_index + 2;
                    if (colorIndex < attrib.colors.size()) {
                        vertex.color = {
                                attrib.colors[colorIndex - 2],
                                attrib.colors[colorIndex - 1],
                                attrib.colors[colorIndex - 0],
                        };
                    } else vertex.color = {1.f, 1.f, 1.f};

                }
                if (index.normal_index >= 0) {
                    vertex.normal = {
                            attrib.normals[3 * index.normal_index + 0],
                            attrib.normals[3 * index.normal_index + 1],
                            attrib.normals[3 * index.normal_index + 2],
                    };
                }
                if (index.texcoord_index >= 0) {
                    vertex.uv = {
                            attrib.texcoords[2 * index.texcoord_index + 0],
                            attrib.texcoords[2 * index.texcoord_index + 1],
                    };
                }
                // check if this vertex is unique
                if (uniqueVertices.count(vertex) == 0) {
                    uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                }
                indices.push_back(uniqueVertices[vertex]);
            }
        }
    }

}
#pragma once

#include "device.hpp"

#include <unordered_map>
#include <memory>
#include <cassert>

namespace tnk {

    class TnkDescriptorSetLayout {
    public:
        class Builder {
        public:
            Builder(TnkDevice &tnkDevice) : tnkDevice{tnkDevice} {}

            Builder &addBinding(
                    uint32_t binding,
                    VkDescriptorType descriptorType,
                    VkShaderStageFlags stageFlags,
                    uint32_t count = 1);
            std::unique_ptr<TnkDescriptorSetLayout> build() const;

        private:
            TnkDevice &tnkDevice;
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
        };

        TnkDescriptorSetLayout(
                TnkDevice &tnkDevice, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
        ~TnkDescriptorSetLayout();
        TnkDescriptorSetLayout(const TnkDescriptorSetLayout &) = delete;
        TnkDescriptorSetLayout &operator=(const TnkDescriptorSetLayout &) = delete;

        VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

    private:
        TnkDevice &tnkDevice;
        VkDescriptorSetLayout descriptorSetLayout;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

        friend class TnkDescriptorWriter;
    };




    class TnkDescriptorPool {
    public:
        class Builder {
        public:
            Builder(TnkDevice &tnkDevice) : tnkDevice{tnkDevice} {}

            Builder &addPoolSize(VkDescriptorType descriptorType, uint32_t count);
            Builder &setPoolFlags(VkDescriptorPoolCreateFlags flags);
            Builder &setMaxSets(uint32_t count);
            std::unique_ptr<TnkDescriptorPool> build() const;

        private:
            TnkDevice &tnkDevice;
            std::vector<VkDescriptorPoolSize> poolSizes{};
            uint32_t maxSets = 1000;
            VkDescriptorPoolCreateFlags poolFlags = 0;
        };

        TnkDescriptorPool(
                TnkDevice &tnkDevice,
                uint32_t maxSets,
                VkDescriptorPoolCreateFlags poolFlags,
                const std::vector<VkDescriptorPoolSize> &poolSizes);
        ~TnkDescriptorPool();
        TnkDescriptorPool(const TnkDescriptorPool &) = delete;
        TnkDescriptorPool &operator=(const TnkDescriptorPool &) = delete;

        bool allocateDescriptorSet(
                const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const;

        void freeDescriptors(std::vector<VkDescriptorSet> &descriptors) const;

        void resetPool();

    private:
        TnkDevice &tnkDevice;
        VkDescriptorPool descriptorPool;

        friend class TnkDescriptorWriter;
    };




    class TnkDescriptorWriter {
    public:
        TnkDescriptorWriter(TnkDescriptorSetLayout &setLayout, TnkDescriptorPool &pool);

        TnkDescriptorWriter &writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo);
        TnkDescriptorWriter &writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo);

        bool build(VkDescriptorSet &set);
        void overwrite(VkDescriptorSet &set);

    private:
        TnkDescriptorSetLayout &setLayout;
        TnkDescriptorPool &pool;
        std::vector<VkWriteDescriptorSet> writes;
    };

}

#pragma once

#include "tnk_device.hpp"


#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <cassert>

namespace tnk {

    static const std::string shaderDir = "../shaders/";

    struct PipeLineConfigInfo {
        VkViewport viewport;
        VkRect2D scissor;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };

    class TnkPipeline {
    public:
        TnkPipeline(TnkDevice &device,
                    const PipeLineConfigInfo &configInfo,
                    const std::string &vertFilePath,
                    const std::string &fragFilePath);

        ~TnkPipeline();

        // no accidental copies
        TnkPipeline(const TnkPipeline &) = delete;

        void operator=(const TnkPipeline &) = delete;

        void bind(VkCommandBuffer commandBuffer);

        static PipeLineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

    private:
        static std::vector<char> readFile(const std::string &filePath);

        void createGraphicsPipeline(const PipeLineConfigInfo &configInfo,
                                    const std::string &vertFilePath,
                                    const std::string &fragFilePath);

        void createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule);

        TnkDevice &tnkDevice; // can leave a dangling pointer, but because a pipeline needs a device to exist, it will not be unsafe (aggregation)
        VkPipeline graphicsPipeline;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;
    };
}
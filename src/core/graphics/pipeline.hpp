#pragma once

#include "device.hpp"
#include "model.hpp"
#include "../tnk_constants.hpp"

#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <cassert>

namespace tnk {

    struct PipeLineConfigInfo {
        PipeLineConfigInfo(const PipeLineConfigInfo &) = delete;
        PipeLineConfigInfo &operator=(const PipeLineConfigInfo &) = delete;

        VkPipelineViewportStateCreateInfo viewportInfo;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        std::vector<VkDynamicState> dynamicStateEnables;
        VkPipelineDynamicStateCreateInfo dynamicStateInfo;
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

        TnkPipeline &operator=(const TnkPipeline &) = delete;

        void bind(VkCommandBuffer commandBuffer);

        static void defaultPipelineConfigInfo(PipeLineConfigInfo &configInfo);

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
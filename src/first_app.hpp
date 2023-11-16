#pragma once

#include "tnk_window.hpp"
#include "tnk_pipeline.hpp"
#include "tnk_device.hpp"
#include "tnk_swap_chain.hpp"

#include <memory>
#include <vector>

namespace tnk
{
	class FirstApp
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

        FirstApp();
        ~FirstApp();

        FirstApp(const FirstApp &) = delete;
        FirstApp &operator=(const FirstApp &) = delete;

		void run();
        float fps();

	private:
        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void drawFrame();

        int tnkFps;

		TnkWindow tnkWindow{ WIDTH, HEIGHT, "Hello Vulkan!" };
        TnkDevice tnkDevice{tnkWindow};
        TnkSwapChain tnkSwapChain{tnkDevice, tnkWindow.getExtent()};
        std::unique_ptr<TnkPipeline> tnkPipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
	};
}
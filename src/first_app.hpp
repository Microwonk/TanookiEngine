#pragma once

#include "core/tnk_window.hpp"
#include "core/tnk_device.hpp"
#include "core/tnk_renderer.hpp"
#include "core/tnk_model.hpp"
#include "core/tnk_game_object.hpp"
#include "core/simple_render_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE // openGL uses -1 to 0, this uses 0 to 1
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

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
        void loadGameObjects();

        int tnkFps;

		TnkWindow tnkWindow{ WIDTH, HEIGHT, "Hello Vulkan!" };
        TnkDevice tnkDevice{tnkWindow};
        TnkRenderer tnkRenderer{tnkWindow, tnkDevice};

        std::vector<TnkGameObject> gameObjects;
	};
}
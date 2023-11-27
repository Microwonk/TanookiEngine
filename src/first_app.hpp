#pragma once

#include "core/tnk_window.hpp"
#include "core/graphics/device.hpp"
#include "core/graphics/renderer.hpp"
#include "core/graphics/model.hpp"
#include "core/tnk_game_object.hpp"
#include "core/systems/simple_render_system.hpp"
#include "core/systems/point_light_system.hpp"
#include "core/graphics/camera.hpp"
#include "core/tnk_controller.hpp"
#include "core/graphics/descriptors.hpp"

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
		static constexpr int WIDTH = 1280;
		static constexpr int HEIGHT = 720;

        FirstApp();
        ~FirstApp();

        FirstApp(const FirstApp &) = delete;
        FirstApp &operator=(const FirstApp &) = delete;

		void run();
        float fps();

	private:
        void loadGameObjects();

        int tnkFps;

		TnkWindow tnkWindow{ WIDTH, HEIGHT, "Debug Game" };
        TnkDevice tnkDevice{tnkWindow};
        TnkRenderer tnkRenderer{tnkWindow, tnkDevice};

        // order of declaration matters (pool needs to be destroyed before the devices) i.e. the deref works from bottom to top
        std::unique_ptr<TnkDescriptorPool> globalPool{};
        TnkGameObject::Map gameObjects;
	};
}
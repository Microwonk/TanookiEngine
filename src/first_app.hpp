#pragma once

#include "tnk_window.hpp"

namespace tnk
{
	class FirstApp
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		void run();

	private:
		TnkWindow tnkWindow{ WIDTH, HEIGHT, "Hello Vulkan!" };
	};
}
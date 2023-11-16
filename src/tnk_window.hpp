#pragma once

#define GLFW_INCLUDE_VULKAN
#include <string>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <chrono>

namespace tnk {
	class TnkWindow	{
	public:
		TnkWindow(int w, int h, std::string name);
		~TnkWindow();

        // no accidental copies
		TnkWindow(const TnkWindow&) = delete;
		TnkWindow& operator=(const TnkWindow&) = delete;

		bool shouldClose() const { return glfwWindowShouldClose(window); }
        VkExtent2D getExtent() { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }
        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

    private:

		void initWindow();

		const int width;
		const int height;

		std::string windowName;

		GLFWwindow* window;
    };
}

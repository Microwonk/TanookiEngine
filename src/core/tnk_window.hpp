#pragma once

#define GLFW_INCLUDE_VULKAN
#include <string>
#include "GLFW/glfw3.h"
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

        void setTitle(std::string title) { glfwSetWindowTitle(window, title.c_str()); }
		bool shouldClose() const { return glfwWindowShouldClose(window); }
        VkExtent2D getExtent() { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }
        bool wasWindowResized() { return frameBufferResized; }
        void resetWindowResizedFlag() { frameBufferResized = false; }
        GLFWwindow *getGLFWwindow() const { return window; }

        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);


    private:
        static void framebufferResizeCallback(GLFWwindow *window, int width, int height);
		void initWindow();

		int width;
		int height;
        bool frameBufferResized;

		std::string windowName;

		GLFWwindow* window;
    };
}

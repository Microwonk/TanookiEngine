#pragma once

#define GLFW_INCLUDE_VULKAN
#include <string>
#include <GLFW/glfw3.h>

namespace tnk {
	class TnkWindow	{
	public:
		TnkWindow(int w, int h, std::string name);
		~TnkWindow();

		TnkWindow(const TnkWindow&) = delete;
		TnkWindow& operator=(const TnkWindow&) = delete;

		bool shouldClose() const { return glfwWindowShouldClose(window); }

	private:

		void initWindow();

		const int width;
		const int height;

		std::string windowName;


		GLFWwindow* window;
	};
}

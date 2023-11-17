#include "tnk_window.hpp"

namespace tnk
{

	TnkWindow::TnkWindow(int w, int h, std::string name) : width{w}, height{h}, windowName{name} {
		initWindow();
	}

	TnkWindow::~TnkWindow()	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}


	void TnkWindow::initWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	}

    void TnkWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface");
        }
    }

    void TnkWindow::framebufferResizeCallback(GLFWwindow *window, int width, int height) {
        auto tnkWindow = reinterpret_cast<TnkWindow*>(glfwGetWindowUserPointer(window));
        tnkWindow->frameBufferResized = true;
        tnkWindow->width = width;
        tnkWindow->height = height;
    }
}
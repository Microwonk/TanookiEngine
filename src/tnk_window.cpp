#include "tnk_window.hpp"

namespace tnk
{

	TnkWindow::TnkWindow(int w, int h, std::string name) : width{w}, height{h}, windowName{name}
	{
		initWindow();
	}

	TnkWindow::~TnkWindow()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}


	void TnkWindow::initWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
	}


}
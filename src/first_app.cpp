#include "first_app.hpp"

namespace tnk {
	void FirstApp::run() {
		while (!tnkWindow.shouldClose()) {



			glfwPollEvents();
		}
	}
}

#include "Controller.h"
GLFWwindow* Controller::window = nullptr;
Controller* Controller::singleton = nullptr;
std::map<int, int> Controller::keystates;

Controller::Controller() {

}

Controller* Controller::getController() {
	if (singleton == nullptr) {
		singleton = new Controller();
	}
	return singleton;
}

void Controller::setWindow(GLFWwindow* w) {
	window = w;
}

bool Controller::pollKeyDown(int key) {
	return glfwGetKey(window,key) == GLFW_PRESS;
	
}

int Controller::pollKeyState(int key) {
	int state = keystates[key];
	if (state == GLFW_PRESS) {
		keystates[key] = GLFW_REPEAT;
	}
	return state;
}

void Controller::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	keystates[key] = action;
}

glm::vec2 Controller::pollMouseDeltas() {
	double cursorX, cursorY;
	glfwGetCursorPos(window,&cursorX,&cursorY);
	glfwSetCursorPos(window, 0.f, 0.f);
	return glm::vec2(-cursorX, -cursorY);
}

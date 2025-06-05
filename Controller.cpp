#include "Controller.h"
#include <iostream>
GLFWwindow* Controller::window = nullptr;
Controller* Controller::singleton = nullptr;

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

glm::vec2 Controller::pollMouseDeltas() {
	double cursorX, cursorY;
	glfwGetCursorPos(window,&cursorX,&cursorY);
	glfwSetCursorPos(window, 0.f, 0.f);
	return glm::vec2(-cursorX, -cursorY);
}

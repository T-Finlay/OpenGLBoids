#pragma once
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <memory>
class Controller
{
private:
	static Controller* singleton;
	static GLFWwindow* window;
	Controller();
public:
	static Controller* getController();
	static void setWindow(GLFWwindow*);
	bool pollKeyDown(int);
	glm::vec2 pollMouseDeltas();
};
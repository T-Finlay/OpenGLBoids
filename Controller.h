#pragma once
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <memory>
#include <map>
class Controller
{
private:
	static Controller* singleton;
	static GLFWwindow* window;
	Controller();
	static std::map<int, int> keystates;
public:
	static Controller* getController();
	static void setWindow(GLFWwindow*);
	bool pollKeyDown(int);
	int pollKeyState(int);
	static void key_callback(GLFWwindow* window, int key,
		int scancode, int action, int mods);
	glm::vec2 pollMouseDeltas();
	static void cleanup();
};
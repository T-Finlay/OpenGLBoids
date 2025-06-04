#include <iostream>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <string>
#include "Renderer.h"
#include "Controller.h"
#include <chrono>

const int WIDTH = 1280;
const int HEIGHT = 720;

//x,y,z,u,v
float cube [] =
{
	//back face
	-1.f,-1.f,-1.f,1.f,0.f, 
	1.f,-1.f,-1.f,0.f,0.f,
	-1.f,1.f,-1.f,1.f,1.f,

	-1.f,1.f,-1.f,1.f,1.f,
	1.f,-1.f,-1.f,0.f,0.f,
	1.f,1.f,-1.f,0.f,1.f,
	
	//left face
	-1.f,-1.f,-1.f,0.f,0.f,
	-1.f,-1.f,1.f,1.f,0.f,
	-1.f,1.f,1.f,1.f,1.f,

	-1.f,-1.f,-1.f,0.f,0.f,
	-1.f,1.f,1.f,1.f,1.f,
	-1.f,1.f,-1.f,0.f,1.f,

	//right face
	1.f,-1.f,-1.f,1.f,0.f,
	1.f,-1.f,1.f,0.f,0.f,
	1.f,1.f,1.f,0.f,1.f,

	1.f,-1.f,-1.f,1.f,0.f,
	1.f,1.f,1.f,0.f,1.f,
	1.f,1.f,-1.f,1.f,1.f,

	//front face
	-1.f,-1.f,1.f,0.f,0.f,
	1.f,-1.f,1.f,1.f,0.f,
	-1.f,1.f,1.f,0.f,1.f,

	-1.f,1.f,1.f,0.f,1.f,
	1.f,-1.f,1.f,1.f,0.f,
	1.f,1.f,1.f,1.f,1.f,

	//top face
	-1.f,1.f,1.f,0.f,0.f,
	1.f,1.f,1.f,1.f,0.f,
	1.f,1.f,-1.f,1.f,1.f,

	-1.f,1.f,1.f,0.f,0.f,
	1.f,1.f,-1.f,1.f,1.f,
	-1.f,1.f,-1.f,0.f,1.f,

	//bottom face
	-1.f,-1.f,1.f,0.f,1.f,
	1.f,-1.f,1.f,1.f,1.f,
	1.f,-1.f,-1.f,1.f,0.f,

	-1.f,-1.f,1.f,0.f,1.f,
	1.f,-1.f,-1.f,1.f,0.f,
	-1.f,-1.f,-1.f,0.f,0.f
};

int main()
{
	auto currentTime = std::chrono::system_clock::now();
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Hello OpenGL", NULL, NULL);
	Controller::setWindow(window);
	glfwMakeContextCurrent(window);
	gl3wInit();
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(window, 0.0, 0.0);
	Renderer renderer(WIDTH,HEIGHT);
	renderer.initialise(cube,sizeof(cube));

	while (!glfwWindowShouldClose(window)) {
		auto newTime = std::chrono::system_clock::now();
		auto deltaTime = newTime - currentTime;
		currentTime = newTime;
		renderer.drawFrame(std::chrono::duration_cast<std::chrono::milliseconds>(deltaTime).count());
		glfwSwapBuffers(window);
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}
	}
	glfwDestroyWindow(window);
	glfwTerminate();
}
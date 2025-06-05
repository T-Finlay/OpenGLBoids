#include <iostream>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <string>
#include "Renderer.h"
#include "Controller.h"
#include <chrono>
#include <iomanip>


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
	std::string title = "Boids Project";
	auto currentTime = std::chrono::high_resolution_clock::now();
	auto lastFPSUpdate = currentTime;
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES,4);
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, title.c_str(), NULL, NULL);
	Controller::setWindow(window);
	glfwMakeContextCurrent(window);
	gl3wInit();
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(window, 0.0, 0.0);
	glfwSetKeyCallback(window,Controller::key_callback);
	Renderer renderer(WIDTH,HEIGHT);
	renderer.initialise(cube,sizeof(cube));

	while (!glfwWindowShouldClose(window)) {
		auto newTime = std::chrono::high_resolution_clock::now();
		auto deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(newTime - currentTime).count();
		currentTime = newTime;
		renderer.drawFrame((float)deltaTime / 1000.f);
		glfwSwapBuffers(window);
		glfwPollEvents();

		if (std::chrono::duration_cast<std::chrono::seconds>(currentTime - lastFPSUpdate).count() > 1) {
			title = ("Boids Project - FPS:  " + std::to_string((int) ((float)(1.f / (float)deltaTime) * 1000000.f)));
			glfwSetWindowTitle(window, title.c_str());
			lastFPSUpdate = currentTime;
		}

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}
	}
	glfwDestroyWindow(window);
	glfwTerminate();
}
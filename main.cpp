#include <iostream>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <string>
#include "Renderer.h"

GLfloat vertices[] =
{
	//pos			
	0.f,  0.5f,
	-0.5f, -0.5f,
	0.5f,  -0.5f
};


int main()
{
	//setup the window
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Hello OpenGL", NULL, NULL);
	glfwMakeContextCurrent(window);
	gl3wInit();
	Renderer r;
	r.initialise(vertices,sizeof(vertices));

	//main loop
	while (!glfwWindowShouldClose(window)) {
		r.drawFrame();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//cleanup
	glfwDestroyWindow(window);
	glfwTerminate();
}
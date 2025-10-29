#include <iostream>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <string>
#include "Renderer.h"
#include "Controller.h"
#include "TextureFactory.h"
#include <iomanip>


const int WIDTH = 1280;
const int HEIGHT = 720;

//x,y,z,u,v
float cubeOld [] =
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
	std::shared_ptr<GeometryLoader> loader(new GeometryLoader());
	loader->loadObjFile("shark.obj");
	int geometrySize = loader->getNumVertexFloats();
	int indicesSize = loader->getNumIndices();
	std::unique_ptr<float> vertices = loader->getVertices();
	std::unique_ptr<unsigned int> indices = loader->getIndices();
	
	double currentTime = glfwGetTime();
	double lastFPSUpdate = currentTime;
	int nbFrames = 0;
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES,4);
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, title.c_str(), NULL, NULL);
	Controller::setWindow(window);
	glfwMakeContextCurrent(window);
	gl3wInit();
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(window, 0.0, 0.0);
	glfwSwapInterval(0);
	glfwSetKeyCallback(window,Controller::key_callback);
	Renderer renderer(WIDTH,HEIGHT,loader);
	renderer.initialise(vertices.get(), geometrySize,indices.get(),indicesSize);
	

	while (!glfwWindowShouldClose(window)) {
		double newTime = glfwGetTime();
		double deltaTime = newTime - currentTime;
		currentTime = newTime;

		renderer.drawFrame((float)deltaTime * 1000);
		glfwSwapBuffers(window);
		glfwPollEvents();
		nbFrames++;

		if (currentTime - lastFPSUpdate > 1) {
			title = ("Boids Project - FPS:  " + std::to_string(nbFrames));
			glfwSetWindowTitle(window, title.c_str());
			lastFPSUpdate = currentTime;
			nbFrames = 0;
		}

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}
	}

	TextureFactory::cleanup();
	Controller::cleanup();

	glfwDestroyWindow(window);
	glfwTerminate();
}
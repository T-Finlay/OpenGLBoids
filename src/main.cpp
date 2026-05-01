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

void glInitialise(std::string title,GLFWwindow** window) {
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 4);
	*window = glfwCreateWindow(WIDTH, HEIGHT, title.c_str(), NULL, NULL);
	Controller::setWindow(*window);
	glfwMakeContextCurrent(*window);
	gl3wInit();
	glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(*window, 0.0, 0.0);
	glfwSwapInterval(0);
	glfwSetKeyCallback(*window, Controller::key_callback);
}

int main() {
	std::string title = "Boids Project";
	GLFWwindow* window;
	double currentTime = glfwGetTime();
	double lastFPSUpdate = currentTime;
	int nbFrames = 0;
	GLuint boidBuffer;
	//the order here is actually very important
	glInitialise(title,&window);
	std::shared_ptr<GeometryLoader> mainLoader(new GeometryLoader());
	std::shared_ptr<EntityManager> entityManager(new EntityManager());
	Renderer renderer(WIDTH, HEIGHT, mainLoader);
	entityManager->createSceneEntities(&boidBuffer);
	entityManager->preInitialiseScene(&renderer,mainLoader);
	renderer.initialise(entityManager);
	entityManager->startScene(&renderer);

	while (!glfwWindowShouldClose(window)) {
		double newTime = glfwGetTime();
		double deltaTime = newTime - currentTime;
		currentTime = newTime;

		entityManager->updateScene(deltaTime);
		renderer.drawFrame((float)deltaTime * 1000,entityManager);
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
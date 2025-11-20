#include "Renderer.h"
#include "EntityManager.h"
#include "DefaultDraw.h"
#include "stdio.h"
#include <stdlib.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <string>
#include <iostream>

float axisLines[] = {
	//x axis
	0.f,0.f,0.f,
	5.f,0.f,0.f,

	//y axis
	0.f,0.f,0.f,
	0.f,5.f,0.f,

	//z axis
	0.f,0.f,0.f,
	0.f,0.f,5.f
};

glm::vec3 red = glm::vec3(1.f, 0.f, 0.f);
glm::vec3 green = glm::vec3(0.f, 1.f, 0.f);
glm::vec3 blue = glm::vec3(0.f, 0.f, 1.f);

void Renderer::initialise(std::shared_ptr<EntityManager> entityManager) {
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(Renderer::DebugCallBack, 0);

	compileShaders();
	createBuffers();
	setupBuffers(entityManager);

	glEnable(GL_DEPTH_TEST);
	
	cam.reset(new Camera(glm::vec3(0.f, 0.f, 10.f), glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 0.f),width,height));
}

void Renderer::drawFrame(float deltaTimeMs, std::shared_ptr<EntityManager> entityManager) {
	glDepthFunc(GL_LESS);
	cam->update(deltaTimeMs);

	glViewport(0, 0, width, height);
	static const GLfloat bgd[] = { .8f, .8f, .8f, 1.f };
	glClearBufferfv(GL_COLOR, 0, bgd);
	glClear(GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	drawAllEntities(entityManager);
	linesShader->useShader();
	drawAxisLines();
}

Renderer::Renderer(int w, int h, std::shared_ptr<GeometryLoader> l) {
	width = w;
	height = h;
	loader = l;
}

void Renderer::DebugCallBack(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	if (type == GL_DEBUG_TYPE_ERROR) {
		fprintf(stderr, "DebugCallback: ERROR - %s\n", message);
	}
}

void Renderer::compileShaders() {
	mainShader.reset(new Shader("basic.vert", "basic.frag"));
	linesShader.reset(new Shader("lines.vert", "lines.frag"));
	mainShader->useShader();
}

std::shared_ptr<Shader> Renderer::getMainShader()
{
	return mainShader;
}

GLuint Renderer::getMainVao() {
	return mainVAO;
}

ModelRenderData Renderer::getModelData(std::string model) {
	return loader->getModelRenderData(model);
}

void Renderer::bindMainVAO() {
	glBindVertexArray(mainVAO);
}

void Renderer::createBuffers() {
	GLuint vaos[2];
	glGenVertexArrays(2, vaos);
	mainVAO = vaos[0];
	linesVAO = vaos[1];

	GLuint buffers[3];
	glCreateBuffers(3, buffers);
	mainBuffer = buffers[0];
	mainEBO = buffers[1];
	linesBuffer = buffers[2];
}

void Renderer::drawAxisLines() {
	glDepthFunc(GL_ALWAYS);
	glBindVertexArray(linesVAO);
	glm::mat4 model = glm::mat4(1.f);
	model = glm::translate(model, glm::vec3(0.f, 0.f, 0.f));
	glUniformMatrix4fv(linesShader->uniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(linesShader->uniformLocation("view"), 1, GL_FALSE, glm::value_ptr(cam->viewMatrix));
	glUniformMatrix4fv(linesShader->uniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(cam->projectionMatrix));

	GLint lColourLocation = linesShader->uniformLocation("lColour");
	glUniform3fv(lColourLocation, 1, glm::value_ptr(red));
	glDrawArrays(GL_LINE_STRIP, 0, 2);

	glUniform3fv(lColourLocation, 1, glm::value_ptr(green));
	glDrawArrays(GL_LINE_STRIP, 2, 2);

	glUniform3fv(lColourLocation, 1, glm::value_ptr(blue));
	glDrawArrays(GL_LINE_STRIP, 4, 2);
}

void Renderer::setupBuffers(std::shared_ptr<EntityManager> entityManager) {
	std::unique_ptr<float> geometry = loader->getVertices();
	int geometrySize = loader->getNumVertexFloats();
	std::unique_ptr<unsigned int> indicies = loader->getIndices();
	int indiciesSize = loader->getNumIndices();
	glBindVertexArray(mainVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mainBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mainEBO);

#ifdef DEBUG_MESH_DATA
	std::cout << "debug vertices" << std::endl;
	int floatN = 0;
	for (int vtxN = 0;vtxN < geometrySize / 8;vtxN++) {
		float x = geometry[floatN++], y = geometry[floatN++], z = geometry[floatN++],
			u = geometry[floatN++], v = geometry[floatN++], nx = geometry[floatN++],
			ny = geometry[floatN++], nz = geometry[floatN++];
		printf("v%d - x: %f, y: %f, z: %f, u: %f, v: %f, nx: %f, ny: %f, nz: %f\n", vtxN,x,y,z,u,v,nx,ny,nz);
	}
	
	std::cout << "debug indicies" << std::endl;
	int intN = 0;
	for (int triN = 0;triN < indiciesSize / 3;triN++) {
		unsigned int v1 = indicies[intN++], v2 = indicies[intN++], v3 = indicies[intN++];
		printf("tri%d - v1:%u, v2:%u, v3:%u\n",triN, v1, v2, v3);
	}
#endif

	glNamedBufferStorage(mainBuffer, geometrySize * sizeof(float), geometry.get(), 0);
	glNamedBufferStorage(mainEBO, indiciesSize * sizeof(unsigned int), indicies.get(), 0);

	//x y z
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//u v
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//nX nY nZ
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glEnable(GL_DEPTH_TEST);

	glBindVertexArray(linesVAO);
	glBindBuffer(GL_ARRAY_BUFFER, linesBuffer);

	glNamedBufferStorage(linesBuffer, sizeof(axisLines), axisLines, 0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void Renderer::drawAllEntities(std::shared_ptr<EntityManager> entityManager) {
	EntityIterator current = entityManager->getBegin();
	EntityIterator end = entityManager->getEnd();

	while (current != end) {
		current->get()->draw(cam,this);
		current++;
	}
}
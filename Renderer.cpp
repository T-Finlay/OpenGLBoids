#include "Renderer.h"
#include "EntityManager.h"
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

void Renderer::initialise(float* geometry, int geometrySize,unsigned int* indicies, int indiciesSize ) {
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(Renderer::DebugCallBack, 0);

	mainShader.reset(new Shader("basic.vert", "basic.frag"));
	linesShader.reset(new Shader("lines.vert", "lines.frag"));
	mainShader->useShader();

	setupBuffers(geometry, geometrySize,indicies,indiciesSize);

	glEnable(GL_DEPTH_TEST);
	
	cam.reset(new Camera(glm::vec3(0.f, 0.f, 10.f), glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 0.f),width,height));
	initTestModel();
}

void Renderer::drawFrame(float deltaTimeMs) {
	glDepthFunc(GL_LESS);
	cam->update(deltaTimeMs);

	glViewport(0, 0, width, height);
	static const GLfloat bgd[] = { .8f, .8f, .8f, 1.f };
	glClearBufferfv(GL_COLOR, 0, bgd);
	glClear(GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	mainShader->useShader();
	drawAllEntities();
	//drawEntity(testCube.get());
	linesShader->useShader();
	drawAxisLines();
}

Renderer::Renderer(int w, int h, std::shared_ptr<GeometryLoader> l) {
	width = w;
	height = h;
	loader = l;
	
	//maybe move this in the future:
	entityManager = std::make_shared<EntityManager>(*(new EntityManager()));
}

void Renderer::DebugCallBack(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	if (type == GL_DEBUG_TYPE_ERROR) {
		fprintf(stderr, "DebugCallback: ERROR - %s\n", message);
	}
}

glm::mat4 Renderer::generateModelMatrix(Entity* e) {
	glm::mat4 modelMatrix = glm::mat4(1.f);
	modelMatrix = glm::translate(modelMatrix, e->position);
	modelMatrix = glm::rotate(modelMatrix, e->rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, e->rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, e->rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::scale(modelMatrix, e->scale);
	return modelMatrix;
}

void Renderer::initTestModel() {
	ModelRenderData testData = loader->getModelRenderData("shark.obj");
	Entity* testObj = new Entity(testData.firstIndexIndex, testData.numIndices,VAOs[0],"shark.jpg",
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(1.f, 1.f, 1.f)
	);
	entityManager->addEntity(testObj);
}

void Renderer::drawAxisLines() {
	glDepthFunc(GL_ALWAYS);
	glBindVertexArray(VAOs[1]);
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

void Renderer::setupBuffers(float* geometry,int geometrySize, unsigned int* indicies, int indiciesSize) {
	glGenVertexArrays(NUM_VAOS, VAOs);
	glCreateBuffers(NUM_BUFFERS, Buffers);
	glCreateBuffers(NUM_EBOS, EBOs);

	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);

#ifdef OUTPUT_MESH_DATA
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

	glNamedBufferStorage(Buffers[0], geometrySize * sizeof(float), geometry, 0);
	glNamedBufferStorage(EBOs[0], indiciesSize * sizeof(unsigned int), indicies, 0);

	//x y z
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//u v
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//nX nY nZ
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glEnable(GL_DEPTH_TEST);

	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);

	glNamedBufferStorage(Buffers[1], sizeof(axisLines), axisLines, 0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void Renderer::drawAllEntities() {
	EntityIterator current = entityManager->getBegin();
	EntityIterator end = entityManager->getEnd();

	while (current != end) {
		drawEntity(current->get());
		current++;
	}
}

void Renderer::drawEntity(Entity* e) {
	if (e == nullptr) {
		std::cout << "attempting to draw null entity, returning " << std::endl;
		return;
	}
	glBindVertexArray(e->VAOIndex);

	glm::mat4 modelMatrix = generateModelMatrix(e);
	glUniformMatrix4fv(linesShader->uniformLocation("model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(linesShader->uniformLocation("view"), 1, GL_FALSE, glm::value_ptr(cam->viewMatrix));
	glUniformMatrix4fv(linesShader->uniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(cam->projectionMatrix));
	e->texture->bind();

	glDrawElements(GL_TRIANGLES, (e->indexCount), GL_UNSIGNED_INT, (const void*)(e->firstIndexIndex));
}
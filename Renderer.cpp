#include "Renderer.h"
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

void Renderer::initialise(float* geometry, int geometrySize) {
	currentTex = new Texture("test_texture.png");

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(Renderer::DebugCallBack, 0);

	mainShader.reset(new Shader("basic.vert", "basic.frag"));
	linesShader.reset(new Shader("lines.vert", "lines.frag"));
	mainShader->useShader();

	//setup buffers
	glGenVertexArrays(NUM_VAOS, VAOs);
	glCreateBuffers(NUM_BUFFERS, Buffers);

	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);

	glNamedBufferStorage(Buffers[0], geometrySize, geometry,0);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glEnable(GL_DEPTH_TEST);

	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);

	glNamedBufferStorage(Buffers[1], sizeof(axisLines), axisLines, 0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);

	cam.reset(new Camera(glm::vec3(0.f, 0.f, 10.f), glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 0.f)));
}

void Renderer::drawFrame(float deltaTimeMs) {
	glDepthFunc(GL_LESS);
	cam->update(deltaTimeMs);
	mainShader->useShader();

	glm::mat4 model, view, projection;
	model = glm::mat4(1.f);
	model = glm::translate(model,glm::vec3(0.f,0.f,0.f));

	view = glm::mat4(1.f);
	view = glm::lookAt(cam->position,cam->position + cam->forward,cam->up);
	projection = glm::mat4(1.f);
	projection = glm::perspective(glm::radians(45.f), (float)width / (float)height, .01f, 500.f);

	static const GLfloat bgd[] = { 1.f, 1.f, 1.f, 1.f };
	glClearBufferfv(GL_COLOR, 0, bgd);
	glClear(GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(VAOs[0]);
	glUniformMatrix4fv(mainShader->uniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(mainShader->uniformLocation("view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(mainShader->uniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));
	currentTex->bind();

	//for test cube
	glDrawArrays(GL_TRIANGLES,0,36);

	linesShader->useShader();
	
	glDepthFunc(GL_ALWAYS);
	glBindVertexArray(VAOs[1]);
	model = glm::mat4(1.f);
	model = glm::translate(model, glm::vec3(0.f, 0.f, 0.f));
	glUniformMatrix4fv(linesShader->uniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(linesShader->uniformLocation("view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(linesShader->uniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));

	GLint lColourLocation = linesShader->uniformLocation("lColour");
	glUniform3fv(lColourLocation,1,glm::value_ptr(red));
	glDrawArrays(GL_LINE_STRIP, 0, 2);

	glUniform3fv(lColourLocation, 1, glm::value_ptr(green));
	glDrawArrays(GL_LINE_STRIP, 2, 2);

	glUniform3fv(lColourLocation, 1, glm::value_ptr(blue));
	glDrawArrays(GL_LINE_STRIP, 4, 2);
}

Renderer::Renderer(int w, int h) {
	width = w;
	height = h;
}

void Renderer::DebugCallBack(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	if (type == GL_DEBUG_TYPE_ERROR) {
		fprintf(stderr, "DebugCallback: ERROR - %s\n", message);
	}
}
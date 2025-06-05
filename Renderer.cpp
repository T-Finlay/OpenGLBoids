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

unsigned int Renderer::compileShader(char* vertexShaderName, char* fragmentShaderName) {
	//setup the shader
	int success;
	char infoLog[512];
	std::string vPath = "shaders/" + std::string(vertexShaderName);
	std::string fPath = "shaders/" + std::string(fragmentShaderName);
	char* vertexSource = readFile(vPath.c_str());
	char* fragmentSource = readFile(fPath.c_str());

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		fprintf(stderr, "Vertex Shader Compilation Fail - %s\n", infoLog);
	}
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		fprintf(stderr, "Fragment Shader Compilation Fail - %s\n", infoLog);
	}
	unsigned int program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		fprintf(stderr, "Shader Program Link Fail - %s\n", infoLog);
	}

	delete vertexSource;
	delete fragmentSource;

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}

void Renderer::initialise(float* geometry, int geometrySize) {
	currentTex = new Texture("test_texture.png");

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(Renderer::DebugCallBack, 0);

	shaderProgram = compileShader("basic.vert", "basic.frag");
	linesProgram = compileShader("lines.vert", "lines.frag");
	glUseProgram(shaderProgram);

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
	glUseProgram(shaderProgram);

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
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"),1,GL_FALSE,glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	currentTex->bind();

	//for test cube
	glDrawArrays(GL_TRIANGLES,0,36);

	glUseProgram(linesProgram);
	
	glDepthFunc(GL_ALWAYS);
	glBindVertexArray(VAOs[1]);
	model = glm::mat4(1.f);
	model = glm::translate(model, glm::vec3(0.f, 0.f, 0.f));
	glUniformMatrix4fv(glGetUniformLocation(linesProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(linesProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(linesProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glUniform3fv(glGetUniformLocation(linesProgram, "lColour"),1,glm::value_ptr(red));
	glDrawArrays(GL_LINE_STRIP, 0, 2);

	glUniform3fv(glGetUniformLocation(linesProgram, "lColour"), 1, glm::value_ptr(green));
	glDrawArrays(GL_LINE_STRIP, 2, 2);

	glUniform3fv(glGetUniformLocation(linesProgram, "lColour"), 1, glm::value_ptr(blue));
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

char* Renderer::readFile(const char* filename) {
	FILE* f;
	fopen_s(&f, filename, "rb");
	if (f == NULL)
		return NULL;
	fseek(f, 0, SEEK_END);
	long size = ftell(f);
	rewind(f);
	char* bfr = new char[size + 1];
	if (bfr == NULL)
		return NULL;
	long ret = fread(bfr, 1, size, f);
	if (ret != size)
		return NULL;
	bfr[size] = '\0';
	return bfr;
}
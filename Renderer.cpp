#include "Renderer.h"
#include "stdio.h"
#include <stdlib.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <string>

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

	free(vertexSource);
	free(fragmentSource);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}

void Renderer::initialise(float* geometry, int geometrySize, unsigned int* indicies, int indiciesSize)
{
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(Renderer::DebugCallBack, 0);

	this->shaderProgram = this->compileShader("basic.vert", "basic.frag");
	glUseProgram(shaderProgram);

	//setup buffers
	glGenVertexArrays(NUM_VAOS, VAOs);
	glCreateBuffers(NUM_BUFFERS, Buffers);
	glCreateBuffers(NUM_EBOS, EBOs);

	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);

	glNamedBufferStorage(Buffers[0], geometrySize, geometry,0);
	glNamedBufferStorage(EBOs[0], indiciesSize, indicies, 0);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void Renderer::drawFrame()
{
	glm::mat4 model, view, projection;
	model = glm::mat4(1.f);
	model = glm::translate(model,glm::vec3(0.f,0.f,0.f));

	view = glm::mat4(1.f);
	view = glm::lookAt(glm::vec3(0.f,0.f,-10.f), glm::vec3(0.f,0.f,1.f), glm::vec3(0.f, 1.f, 0.f));
	projection = glm::mat4(1.f);
	projection = glm::perspective(glm::radians(45.f), (float)width / (float)height, .01f, 500.f);

	static const GLfloat bgd[] = { 1.f, 1.f, 1.f, 1.f };
	glClearBufferfv(GL_COLOR, 0, bgd);
	glBindVertexArray(VAOs[0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"),1,GL_FALSE,glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
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
	char* bfr = (char*)malloc(sizeof(char) * (size + 1));
	if (bfr == NULL)
		return NULL;
	long ret = fread(bfr, 1, size, f);
	if (ret != size)
		return NULL;
	bfr[size] = '\0';
	return bfr;
}
#include "Renderer.h"
#include "stdio.h"
#include <stdlib.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <string>

unsigned int Renderer::compileShader(char* vertexShaderName, char* fragmentShaderName) {
	//setup the shader
	std::string vPath = "shaders/" + std::string(vertexShaderName);
	std::string fPath = "shaders/" + std::string(fragmentShaderName);
	char* vertexSource = readFile(vPath.c_str());
	char* fragmentSource = readFile(fPath.c_str());

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	unsigned int program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	free(vertexSource);
	free(fragmentSource);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}

void Renderer::initialise(float* geometry, int geometrySize)
{
	this->shaderProgram = this->compileShader("basic.vert", "basic.frag");
	glUseProgram(shaderProgram);

	//setup buffers
	glCreateBuffers(NUM_BUFFERS, Buffers);
	glNamedBufferStorage(Buffers[0], geometrySize, geometry, 0);
	glGenVertexArrays(NUM_VAOS, VAOs);
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);
}

void Renderer::drawFrame()
{
	static const GLfloat bgd[] = { 1.f, 1.f, 1.f, 1.f };
	glClearBufferfv(GL_COLOR, 0, bgd);
	glBindVertexArray(VAOs[0]);
	glDrawArrays(GL_TRIANGLES, 0, 3);
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
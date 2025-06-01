#pragma once
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#define NUM_BUFFERS 1
#define NUM_VAOS 1
class Renderer
{
public:
	unsigned int shaderProgram;
	GLuint Buffers[NUM_BUFFERS];
	GLuint VAOs[NUM_VAOS];
	unsigned int compileShader(char*,char*);
	void initialise(float*,int);
	void drawFrame();

private:
	char* readFile(const char*);
};


#pragma once
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "Texture.h"
#include "Camera.h"
#include <memory>
#define NUM_BUFFERS 2
#define NUM_VAOS 2
class Renderer
{
public:
	unsigned int shaderProgram;
	GLuint Buffers[NUM_BUFFERS];
	GLuint VAOs[NUM_VAOS];
	unsigned int compileShader(char*,char*);
	void initialise(float*,int);
	void drawFrame(float);
	Renderer(int,int);
	int width, height;
	static void APIENTRY DebugCallBack(GLenum, GLenum, 
		GLuint, GLenum, GLsizei, const GLchar*, const void*);

private:
	unsigned int linesProgram;
	char* readFile(const char*);
	Texture* currentTex;
	std::unique_ptr<Camera> cam;
};


#pragma once
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#define NUM_BUFFERS 1
#define NUM_VAOS 1
#define NUM_EBOS 1
class Renderer
{
public:
	unsigned int shaderProgram;
	GLuint Buffers[NUM_BUFFERS];
	GLuint VAOs[NUM_VAOS];
	GLuint EBOs[NUM_EBOS];
	unsigned int compileShader(char*,char*);
	void initialise(float*,int,unsigned int*,int);
	void drawFrame();
	Renderer(int,int);
	int width, height;
	static void APIENTRY DebugCallBack(GLenum, GLenum, 
		GLuint, GLenum, GLsizei, const GLchar*, const void*);

private:
	char* readFile(const char*);
};


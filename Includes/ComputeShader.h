#pragma once
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
class ComputeShader
{
public:
	ComputeShader(char* shaderFileName);
	void useShader();
	GLint uniformLocation(const char*);

private:
	char* readFile(const char*);
	GLuint glShaderID;
};


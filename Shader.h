#pragma once
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
class Shader {
public:
	Shader(char* vertexShaderName, char* fragmentShaderName);
	void useShader();
	GLint uniformLocation(const char*);
private:
	unsigned int glShaderID;
	char* readFile(const char*);
};


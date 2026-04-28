#include "ComputeShader.h"
#include <string>


ComputeShader::ComputeShader(char* shaderFileName) {
	int success;
	GLuint program;
	
	char infoLog[512];
	std::string shaderpath = "shaders/" + std::string(shaderFileName);
	char* shaderSource = readFile(shaderpath.c_str());
	GLuint shader = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		fprintf(stderr, "Compute Shader Shader Compilation Fail - %s\n", infoLog);
	}

	program = glCreateProgram();
	glAttachShader(program, shader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		fprintf(stderr, "Compute Shader Program Link Fail - %s\n", infoLog);
	}

	delete shaderSource;
	glDeleteShader(shader);

	glShaderID = program;
}

GLint ComputeShader::uniformLocation(const char* uniformName) {
	return glGetUniformLocation(glShaderID, uniformName);
}

void ComputeShader::useShader() {
	glUseProgram(glShaderID);
}

char* ComputeShader::readFile(const char* filename) {
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
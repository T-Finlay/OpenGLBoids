#include "Shader.h"
#include <string>

Shader::Shader(char* vertexShaderName, char* fragmentShaderName) {
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

	glShaderID = program;
}

void Shader::useShader() {
	glUseProgram(glShaderID);
}

GLint Shader::uniformLocation(const char* uniformName) {
	return glGetUniformLocation(glShaderID, uniformName);
}

char* Shader::readFile(const char* filename) {
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
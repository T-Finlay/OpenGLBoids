#version 450 core
layout (location = 0) in vec4 vaPos;
void main() {
	gl_Position = vaPos;
}
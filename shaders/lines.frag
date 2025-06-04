#version 450 core
layout (location = 0) out vec4 fColour;

uniform vec3 lColour;

void main() {
	fColour = vec4(lColour,1.0f);
}
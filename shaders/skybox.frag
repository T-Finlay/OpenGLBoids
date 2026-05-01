#version 450 core

layout (location = 0) out vec4 fColour;

in vec3 textureCoords;

uniform samplerCube skybox;

void main() {

	fColour = vec4(texture(skybox,textureCoords).xyz,1.0f);
}
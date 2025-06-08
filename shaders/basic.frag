#version 450 core
layout (location = 0) out vec4 fColour;

uniform sampler2D m_texture;
 
in vec2 uv;

void main() {
	fColour = texture(m_texture,uv);
}
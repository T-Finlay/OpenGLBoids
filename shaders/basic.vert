#version 450 core
layout (location = 0) in vec4 vaPos;
layout (location = 1) in vec2 tex_cords;
layout (location = 2) in vec3 vtxNor;

out vec2 uv;
out vec3 nor;
out vec3 fragPosWorldSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = projection * view * model * vaPos;
	uv = tex_cords;
	fragPosWorldSpace = (model * vaPos).xyz;
	nor = mat3(transpose(inverse(model))) * vtxNor;
}
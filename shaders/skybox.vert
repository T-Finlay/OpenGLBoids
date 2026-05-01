#version 450 core

layout (location = 0) in vec4 vPos;

out vec3 textureCoords;

uniform mat4 projection;
uniform mat4 view; 

void main() {
	vec4 pos = (projection * view * vec4(vPos.xyz,1.0f));
	gl_Position = vec4(pos.x,pos.y,pos.w,pos.w);
	//by forcing the z and w components like this, we ensure that the skybox will always be drawn at max depth
	textureCoords = vec3(vPos.x,vPos.y,-vPos.z);
}
#version 450 core

layout (location = 0) in vec4 vaPos;
layout (location = 1) in vec2 tex_coords;
layout (location = 2) in vec3 vtxNor;


struct boidData {
	vec4 position;
	vec4 direction;
};

layout (binding = 0,std430) readonly buffer boidsData {
	boidData dat[];
};

out vec2 uv;
out vec3 nor;
out vec3 fragPosWorldSpace;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 scale;

mat4 modelMatrix() {
	mat4 translationMatrix = mat4(0.0f);
	mat4 rotationMatrixPitch = mat4(0.0f);
	mat4 rotationMatrixYaw = mat4(0.0f);
	if(gl_InstanceID > dat.length()) {
		return mat4(0.0f);
	}
	vec3 boidPos = dat[gl_InstanceID].position.xyz;
	vec3 boidDir = dat[gl_InstanceID].direction.xyz;

	float yaw = atan(boidDir.x,boidDir.z);
	float pitch = atan(boidDir.y,sqrt(pow(boidDir.x,2) + pow(boidDir.z,2)));
	translationMatrix[0][0] = 1.0f;
	translationMatrix[1][1] = 1.0f;
	translationMatrix[2][2] = 1.0f;
	translationMatrix[3][3] = 1.0f;
	translationMatrix[3][0] = boidPos.x;
	translationMatrix[3][1] = boidPos.y;
	translationMatrix[3][2] = boidPos.z;

	rotationMatrixPitch[0][0] = 1.0f;
	rotationMatrixPitch[1][1] = cos(pitch);
	rotationMatrixPitch[2][1] = sin(pitch);
	rotationMatrixPitch[1][2] = -sin(pitch);
	rotationMatrixPitch[2][2] = cos(pitch);
	rotationMatrixPitch[3][3] = 1.0f;

	
	rotationMatrixYaw[0][0] = cos(yaw);
	rotationMatrixYaw[0][2] = -sin(yaw);
	rotationMatrixYaw[1][1] = 1.0f;
	rotationMatrixYaw[2][0] = sin(yaw);
	rotationMatrixYaw[2][2] = cos(yaw);
	rotationMatrixYaw[3][3] = 1.0f;
	
	return translationMatrix * rotationMatrixYaw * rotationMatrixPitch;
}

void main() {
	mat4 model = modelMatrix() * scale;
	gl_Position = projection * view * model * vaPos;
	uv = tex_coords;
	fragPosWorldSpace = (model * vaPos).xyz;
	nor = mat3(transpose(inverse(model))) * vtxNor;
}



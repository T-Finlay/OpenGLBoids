#version 450 core
layout (location = 0) out vec4 fColour;
uniform sampler2D m_texture;
uniform vec3 lightDirection;
uniform vec3 camPos;
in vec2 uv;
in vec3 nor;
in vec3 fragPosWorldSpace;

void main() {
	const float ambientLight = 0.5f;
	vec3 surfaceNormal = normalize(nor);
	vec3 incomingRay = normalize(lightDirection);
	vec4 unlitColour = texture(m_texture,uv);
	float diffuseLight = max(dot(-incomingRay,surfaceNormal),0.0f);

	vec3 reflectedLight = reflect(incomingRay,surfaceNormal);
	vec3 directionToCamera = normalize(camPos - fragPosWorldSpace);

	float specularLight = pow(max(dot(reflectedLight,directionToCamera),0),128);

	fColour = vec4(unlitColour.xyz * (min(ambientLight+diffuseLight+specularLight,1.0f)),unlitColour.w);
}
#include "Entity.h"

Entity::Entity(unsigned int fstVertex, unsigned int nVertex, unsigned int vao, Texture* tex, glm::vec3 pos, glm::vec3 rot, glm::vec3 scl) {
	firstVertixIndex = fstVertex;
	vertexCount = nVertex;
	VAOIndex = vao;
	texture = tex;
	position = pos;
	rotation = rot;
	scale = scl;
}

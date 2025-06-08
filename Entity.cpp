#include "Entity.h"
#include "TextureFactory.h"

Entity::Entity(unsigned int fstVertex, unsigned int nVertex, unsigned int vao, std::string tex, glm::vec3 pos, glm::vec3 rot, glm::vec3 scl) {
	firstVertixIndex = fstVertex;
	vertexCount = nVertex;
	VAOIndex = vao;
	texture = TextureFactory::getTextureFactory()->registerTexture(tex);
	position = pos;
	rotation = rot;
	scale = scl;
}

#include "Entity.h"
#include "TextureFactory.h"

Entity::Entity(unsigned int fstIndex, unsigned int nIndex, unsigned int vao, std::string tex, glm::vec3 pos, glm::vec3 rot, glm::vec3 scl) {
	firstIndexIndex = fstIndex;
	indexCount = nIndex;
	VAOIndex = vao;
	texture = TextureFactory::getTextureFactory()->registerTexture(tex);
	position = pos;
	rotation = rot;
	scale = scl;
}

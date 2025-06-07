#pragma once
#include "texture.h"
#include <glm/glm.hpp>
class Entity {
public:
	unsigned int firstVertixIndex;
	unsigned int vertexCount;
	unsigned int VAOIndex;
	Texture* texture;
	Entity(unsigned int,unsigned int, unsigned int, Texture*, glm::vec3, glm::vec3, glm::vec3);

	//note rotations need to be in radians
	glm::vec3 position, rotation, scale;
};


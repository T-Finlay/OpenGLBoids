#pragma once
#include "texture.h"
#include "TextureFactory.h"
#include <glm/glm.hpp>
#include <memory>
#include <string>
class Entity {
public:
	unsigned int firstIndexIndex;
	unsigned int indexCount;
	unsigned int VAOIndex;
	std::shared_ptr<Texture> texture;
	Entity(unsigned int,unsigned int, unsigned int, 
		std::string, glm::vec3, glm::vec3, glm::vec3);

	//note rotations need to be in radians
	glm::vec3 position, rotation, scale;
};


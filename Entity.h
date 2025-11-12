#pragma once
#include "texture.h"
#include "TextureFactory.h"
#include "Camera.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <memory>
#include <string>

//forward declaration
class DrawRoutine;

class Entity : public std::enable_shared_from_this<Entity> {
public:
	unsigned int firstIndexIndex;
	unsigned int indexCount;
	unsigned int VAOIndex;
	std::shared_ptr<Texture> texture;
	Entity(unsigned int,unsigned int, unsigned int,
		std::string, glm::vec3, glm::vec3, glm::vec3,bool);
	void draw(std::shared_ptr<Camera>);
	void setDrawRoutine(std::unique_ptr<DrawRoutine> newDrawRoutine);
	~Entity();

	//note rotations need to be in radians
	glm::vec3 position, rotation, scale;

private:
	std::unique_ptr<DrawRoutine> drawRoutine;
};


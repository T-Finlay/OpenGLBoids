#include "Entity.h"
#include "TextureFactory.h"
#include "DrawRoutine.h"
#include "DefaultDraw.h"
#include <iostream>

Entity::Entity(unsigned int fstIndex, unsigned int nIndex, unsigned int vao, 
		std::string tex, glm::vec3 pos, glm::vec3 rot, glm::vec3 scl, bool useDefaultDraw) {
	firstIndexIndex = fstIndex;
	indexCount = nIndex;
	VAOIndex = vao;
	texture = TextureFactory::getTextureFactory()->registerTexture(tex);
	position = pos;
	rotation = rot;
	scale = scl;
	if (useDefaultDraw) {
		drawRoutine = DefaultDraw::generateDefaultDraw();
	}
}

void Entity::draw(std::shared_ptr<Camera> cam) {
	if (drawRoutine == nullptr) {
		std::cout << "attempting to draw entity with no draw routine, returning: " << std::endl;
		return;
	}
	drawRoutine->draw(shared_from_this(), cam);
}

void Entity::setDrawRoutine(std::unique_ptr<DrawRoutine> newDrawRoutine) {
	drawRoutine = std::move(newDrawRoutine);
}

Entity::~Entity() {};
#include "Entity.h"
#include "TextureFactory.h"
#include "DrawRoutine.h"
#include "DefaultDraw.h"
#include "Renderer.h"
#include "Behaviour.h"
#include <iostream>

Entity::Entity(glm::vec3 pos, glm::vec3 rot, glm::vec3 scl, std::string modelName, std::string textureName) {
	position = pos;
	rotation = rot;
	scale = scl;
	drawRoutine.reset(new DefaultDraw(modelName, textureName));
}

Entity::Entity(glm::vec3 pos, glm::vec3 rot, glm::vec3 scl) {
	position = pos;
	rotation = rot;
	scale = scl;
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

Entity::~Entity() {}
void Entity::addBehaviour(std::unique_ptr<Behaviour> b) {
	behaviours.push_back(std::move(b));
}

void Entity::update(float deltaTime) {
	for (std::unique_ptr<Behaviour>& behaviour : behaviours) {
		behaviour->update(deltaTime,shared_from_this());
	}
}

void Entity::start(Renderer* r) {
	drawRoutine->initialise(shared_from_this(),r);
	for (std::unique_ptr<Behaviour>& behaviour : behaviours) {
		behaviour->initialise(shared_from_this());
	}
}

#include "EntityManager.h"
#include "Renderer.h"
#include <vector>
#include <memory>
#include <iterator>
#include <fstream>
#include <iostream>
#include "Behaviour.h"
#include "TestBehaviour.h"
#include "BoidsManager.h"
#include "BoidsDrawer.h"


void EntityManager::addEntity(std::shared_ptr<Entity> e) {
	entities.push_back(e);
}

EntityIterator EntityManager::getBegin() {
	return entities.begin();
}

EntityIterator EntityManager::getEnd() {
	return entities.end();
}

//create entity objects for the scene (does not load geometry)
void EntityManager::createSceneEntities(GLuint* boidBuffer) {
	std::shared_ptr<Entity> seaFloor(new Entity(glm::vec3(0.0f, -120.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(200.f, 1.f, 200.f),"plane.obj","sandTexture.jpg"));
	addEntity(seaFloor);

	std::shared_ptr<Entity> boidManager(new Entity(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
	std::unique_ptr<BoidsDrawer> bd(new BoidsDrawer(boidBuffer));
	std::unique_ptr<BoidsManager> boidBehaviour(new BoidsManager(boidBuffer));
	boidManager->addBehaviour(std::move(boidBehaviour));
	boidManager->setDrawRoutine(std::move(bd));
	addEntity(boidManager);
}

//handles all pre initialisation for objects BEFORE renederer itself is initialised
void EntityManager::preInitialiseScene(Renderer* r, std::shared_ptr<GeometryLoader> loader) {
	EntityIterator current = getBegin();
	EntityIterator end = getEnd();

	while (current != end) {
		current->get()->preInitialise(r,loader);
		current++;
	}
}

void EntityManager::startScene(Renderer* r) {
	EntityIterator current = getBegin();
	EntityIterator end = getEnd();

	while (current != end) {
		current->get()->start(r);
		current++;
	}
}

void EntityManager::updateScene(float deltaTime)
{
	EntityIterator current = getBegin();
	EntityIterator end = getEnd();

	while (current != end) {
		current->get()->update(deltaTime);
		current++;
	}
}

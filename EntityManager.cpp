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

void EntityManager::createSceneEntities(GLuint* boidBuffer) {
	//ideally i would have this read a level format from a json file or something
	//but that actually proved much more complex than i initially thought
	//for loading things like positions and rotations, its fine, but if you want it 
	//to use custom draw routines and behaviours loading specific classes which is much more complicated
	//std::unique_ptr<TestBehaviour> testBehaviour(new TestBehaviour());
	//std::shared_ptr<Entity> testObj(new Entity(glm::vec3(0.f, 0.f, 0.f),
		//glm::vec3(0.f, 0.f, 0.f),
		//glm::vec3(1.f, 1.f, 1.f), "shark.obj", "shark.jpg"));
	//testObj->addBehaviour(std::move(testBehaviour));
	//addEntity(testObj);
	std::shared_ptr<Entity> boidManager(new Entity(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
	std::unique_ptr<BoidsDrawer> bd(new BoidsDrawer(boidBuffer));
	std::unique_ptr<BoidsManager> boidBehaviour(new BoidsManager(boidBuffer));
	boidManager->addBehaviour(std::move(boidBehaviour));
	boidManager->setDrawRoutine(std::move(bd));
	addEntity(boidManager);

	//std::shared_ptr<Entity> testFish(new Entity(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.1f, 0.1f, 0.1f), "fish.obj", "fish.jpg"));
	//addEntity(testFish);
}

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

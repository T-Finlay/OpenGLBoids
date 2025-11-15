#include "EntityManager.h"
#include "Renderer.h"
#include <vector>
#include <memory>
#include <iterator>
#include <fstream>
#include <iostream>
#include "Behaviour.h"
#include "TestBehaviour.h"


void EntityManager::addEntity(std::shared_ptr<Entity> e) {
	entities.push_back(e);
}

EntityIterator EntityManager::getBegin() {
	return entities.begin();
}

EntityIterator EntityManager::getEnd() {
	return entities.end();
}

void EntityManager::loadScene(std::shared_ptr<GeometryLoader> loader) {
	//ideally i would have this read a level format from a json file or something
	//but that actually proved much more complex than i initially thought
	//for loading things like positions and rotations, its fine, but if you want it 
	//to use custom draw routines and behaviours loading specific classes which is much more complicated
	loader->loadObjFile("shark.obj");
	/*
	ModelRenderData testData = loader->getModelRenderData("shark.obj");
	std::shared_ptr<Entity> testObj(new Entity(testData.firstIndexIndex, testData.numIndices,  "shark.jpg",
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(1.f, 1.f, 1.f),
		true
	));
	*/
	std::unique_ptr<TestBehaviour> testBehaviour(new TestBehaviour());
	std::shared_ptr<Entity> testObj(new Entity(glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(1.f, 1.f, 1.f), "shark.obj", "shark.jpg"));
	testObj->addBehaviour(std::move(testBehaviour));
	addEntity(testObj);
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

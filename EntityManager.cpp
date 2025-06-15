#include "EntityManager.h"

void EntityManager::addEntity(Entity* e) {
	entities.push_back(std::make_shared<Entity>(*e));
}

EntityIterator EntityManager::getBegin() {
	return entities.begin();
}

EntityIterator EntityManager::getEnd() {
	return entities.end();
}

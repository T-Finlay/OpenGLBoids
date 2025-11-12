#include "EntityManager.h"
#include <vector>
#include <memory>
#include <iterator>

void EntityManager::addEntity(std::shared_ptr<Entity> e) {
	entities.push_back(e);
}

EntityIterator EntityManager::getBegin() {
	return entities.begin();
}

EntityIterator EntityManager::getEnd() {
	return entities.end();
}

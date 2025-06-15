#pragma once
#include <vector>
#include <memory>
#include "Entity.h"
#include <iterator>
typedef std::vector<std::shared_ptr<Entity>>::iterator EntityIterator;

class EntityManager
{
private:
	std::vector<std::shared_ptr<Entity>> entities;
public:
	EntityManager() {}
	void addEntity(Entity*);
	EntityIterator getBegin();
	EntityIterator getEnd();
};


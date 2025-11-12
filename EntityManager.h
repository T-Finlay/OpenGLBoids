#pragma once
#include <vector>
#include <memory>
#include <xutility>
#include "Entity.h"
#include <iterator>
#include "GeometryLoader.h"
typedef std::vector<std::shared_ptr<Entity>>::iterator EntityIterator;

class EntityManager
{
private:
	std::vector<std::shared_ptr<Entity>> entities;

public:
	EntityManager() {}
	void addEntity(std::shared_ptr<Entity>);
	EntityIterator getBegin();
	EntityIterator getEnd();
};


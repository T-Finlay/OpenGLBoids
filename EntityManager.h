#pragma once
#include <vector>
#include <memory>
#include <xutility>
#include "Entity.h"
#include <iterator>
#include "glm/glm.hpp"
#include "GeometryLoader.h"
typedef std::vector<std::shared_ptr<Entity>>::iterator EntityIterator;

typedef struct EntityData {
	std::string modelFile;
	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 scl;
	std::string textureFile;
	
} EntityData;

//forward Declaration
class Renderer;

class EntityManager
{
private:
	std::vector<std::shared_ptr<Entity>> entities;
	

public:
	EntityManager() {}
	void addEntity(std::shared_ptr<Entity>);
	EntityIterator getBegin();
	EntityIterator getEnd();

	void loadScene(std::shared_ptr<GeometryLoader> loader);
	void startScene(Renderer* r);
	void updateScene(float deltaTime);
};


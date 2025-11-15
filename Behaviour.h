#pragma once
#include <memory>

//forward declaration
class Entity;

class Behaviour {
public: 
	virtual void initialise(std::shared_ptr<Entity> self) = 0;
	virtual void update(float deltaTime, std::shared_ptr<Entity> self) = 0;
};
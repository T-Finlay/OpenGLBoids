#include "testBehaviour.h"
#include "Entity.h"
#include "glm/glm.hpp"
#include <iostream>
#define ROTATION_AMOUNT glm::radians(10.f)

void TestBehaviour::update(float deltaTime,std::shared_ptr<Entity> self) {
	//rotate about y axis
	self->rotation.y = fmodf(self->rotation.y + (ROTATION_AMOUNT * deltaTime),glm::radians(360.f));
}

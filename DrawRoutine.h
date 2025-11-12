#pragma once
#include <memory>
#include "Camera.h"
#include "Shader.h"
#include "glm/glm.hpp"
#include <GLM/gtc/matrix_transform.hpp>

//forward declaration
class Entity;

class DrawRoutine
{
public:
	DrawRoutine(std::shared_ptr<Shader>);
	virtual void draw(std::shared_ptr<Entity> self,
		std::shared_ptr<Camera> cam) = 0;
	glm::mat4 generateModelMatrix(Entity* self);
	~DrawRoutine();
protected:
	std::shared_ptr<Shader> shader;
};
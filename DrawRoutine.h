#pragma once
#include <memory>
#include "Camera.h"
#include "Shader.h"
#include "glm/glm.hpp"
#include <GLM/gtc/matrix_transform.hpp>

//forward declaration
class Entity;
class Renderer;

class DrawRoutine
{
public:
	DrawRoutine() {};
	virtual void draw(std::shared_ptr<Entity> self,
		std::shared_ptr<Camera> cam) = 0;
	virtual void initialise(std::shared_ptr<Entity> self, Renderer* renderer) = 0;
	glm::mat4 generateModelMatrix(Entity* self);
	~DrawRoutine();
protected:
	std::shared_ptr<Shader> shader;
};
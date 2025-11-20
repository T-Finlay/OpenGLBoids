#pragma once
#include <memory>
#include "Camera.h"
#include "Shader.h"
#include "glm/glm.hpp"
#include <GLM/gtc/matrix_transform.hpp>

//forward declaration
class Entity;
class Renderer;
class GeometryLoader;

class DrawRoutine
{
public:
	DrawRoutine() {};
	virtual void draw(std::shared_ptr<Entity> self,
		std::shared_ptr<Camera> cam, Renderer* renderer) = 0;

	//pre initialise occcurs before the renderer has completed its initialisation
	//notably this allows for a draw routine to perform any logic required before
	virtual void preInitialise(std::shared_ptr<Entity> self, Renderer* renderer, std::shared_ptr<GeometryLoader> loader) = 0;
	virtual void initialise(std::shared_ptr<Entity> self, Renderer* renderer) = 0;
	glm::mat4 generateModelMatrix(Entity* self);
	~DrawRoutine();
};
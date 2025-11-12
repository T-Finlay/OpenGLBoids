#include "DrawRoutine.h"
#include "Entity.h"

DrawRoutine::DrawRoutine(std::shared_ptr<Shader> s) {
	shader = s;
}

DrawRoutine::~DrawRoutine() {}

glm::mat4 DrawRoutine::generateModelMatrix(Entity* self)
{
	glm::mat4 modelMatrix = glm::mat4(1.f);
	modelMatrix = glm::translate(modelMatrix, self->position);
	modelMatrix = glm::rotate(modelMatrix, self->rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, self->rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, self->rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::scale(modelMatrix, self->scale);
	return modelMatrix;
}


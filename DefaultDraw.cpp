#include "DefaultDraw.h"
#include "Entity.h"
#include <iostream>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <memory>

std::shared_ptr<Shader> DefaultDraw::defaultShader = nullptr;

DefaultDraw::DefaultDraw(std::shared_ptr<Shader> s) : DrawRoutine(s) {}

DefaultDraw::~DefaultDraw() {}

void DefaultDraw::draw(std::shared_ptr<Entity> self, std::shared_ptr<Camera> cam) {
	if (self == nullptr) {
		std::cout << "attempting to draw null entity, returning " << std::endl;
		return;
	}

	if (cam == nullptr) {
		std::cout << "attempting to draw entity with null camera, returning " << std::endl;
		return;
	} 

	glBindVertexArray(self->VAOIndex);

	glm::mat4 modelMatrix = generateModelMatrix(self.get());
	glUniformMatrix4fv(shader->uniformLocation("model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(shader->uniformLocation("view"), 1, GL_FALSE, glm::value_ptr(cam->viewMatrix));
	glUniformMatrix4fv(shader->uniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(cam->projectionMatrix));
	self->texture->bind();

	glDrawElements(GL_TRIANGLES, (self->indexCount), GL_UNSIGNED_INT, (const void*)(self->firstIndexIndex));
}

std::unique_ptr<DefaultDraw> DefaultDraw::generateDefaultDraw() {
	return std::unique_ptr<DefaultDraw>(new DefaultDraw(defaultShader));
}

void DefaultDraw::setDefaultShader(std::shared_ptr<Shader> s) {
	defaultShader = s;
}

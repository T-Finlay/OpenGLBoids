#include "DefaultDraw.h"
#include "Renderer.h"
#include "Entity.h"
#include "Texture.h"
#include "TextureFactory.h"
#include "GeometryLoader.h"
#include <iostream>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <memory>

DefaultDraw::DefaultDraw(std::string model, std::string texture) : DrawRoutine() {
	modelName = model;
	textureName = texture;
}

DefaultDraw::~DefaultDraw() {}

void DefaultDraw::draw(std::shared_ptr<Entity> self, std::shared_ptr<Camera> cam,Renderer* r) {
	if (self == nullptr) {
		std::cout << "attempting to draw null entity, returning " << std::endl;
		return;
	}

	if (cam == nullptr) {
		std::cout << "attempting to draw entity with null camera, returning " << std::endl;
		return;
	} 

	shader->useShader();
	r->bindMainVAO();

	glm::mat4 modelMatrix = generateModelMatrix(self.get());
	glUniformMatrix4fv(shader->uniformLocation("model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(shader->uniformLocation("view"), 1, GL_FALSE, glm::value_ptr(cam->viewMatrix));
	glUniformMatrix4fv(shader->uniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(cam->projectionMatrix));
	texture->bind();
	glDrawElements(GL_TRIANGLES, (indexCount), GL_UNSIGNED_INT, (const void*)(firstIndexIndex));
}

void DefaultDraw::preInitialise(std::shared_ptr<Entity> self, Renderer* renderer,std::shared_ptr<GeometryLoader> loader) {
	loader->loadObjFile(modelName);
	ModelRenderData mrd = renderer->getModelData(modelName);
	firstIndexIndex = mrd.firstIndexIndex;
	indexCount = mrd.numIndices;
	texture = TextureFactory::getTextureFactory()->registerTexture(textureName);
}

void DefaultDraw::initialise(std::shared_ptr<Entity> self, Renderer* renderer) {
	shader = renderer->getMainShader();
}
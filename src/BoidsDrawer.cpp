#include "BoidsDrawer.h"
#include "GeometryLoader.h"
#include "BoidsManager.h"
#include "TextureFactory.h"
#include "glm/glm.hpp"
#include "Shader.h"
#include <GLM/gtc/type_ptr.hpp>
#include "Renderer.h"
//#define DEBUG_SHADER_STORAGE_CONTENT

BoidsDrawer::BoidsDrawer(GLuint* ptr) {
	boidDataBufferPointer = ptr;
}

void BoidsDrawer::draw(std::shared_ptr<Entity> self, 
	std::shared_ptr<Camera> cam, Renderer* renderer) {

	if (self == nullptr) {
		std::cout << "attempting to draw null entity, returning " << std::endl;
		return;
	}

	if (cam == nullptr) {
		std::cout << "attempting to draw entity with null camera, returning " << std::endl;
		return;
	}

	boidShader->useShader();
	glBindVertexArray(boidVAO);
	glm::mat4 scale = glm::mat4(1.f);
	scale = glm::scale(scale, glm::vec3(0.5f, 0.5f, 0.5f));
	glm::vec3 lightDirection = renderer->getLightDirection();
	glUniform3f(boidShader->uniformLocation("lightDirection"), lightDirection.x,lightDirection.y,lightDirection.z);
	glUniform3f(boidShader->uniformLocation("camPos"),cam->position.x,cam->position.y,cam->position.z);
	glUniformMatrix4fv(boidShader->uniformLocation("scale"), 1, GL_FALSE, glm::value_ptr(scale));
	glUniformMatrix4fv(boidShader->uniformLocation("view"), 1, GL_FALSE, glm::value_ptr(cam->viewMatrix));
	glUniformMatrix4fv(boidShader->uniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(cam->projectionMatrix));
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, *boidDataBufferPointer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, *boidDataBufferPointer);

#ifdef DEBUG_SHADER_STORAGE_CONTENT
	debugShaderStorageContent();
#endif

	//drawCall
	texture->bind();
	glDrawElementsInstanced(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, (const void*)firstIndexIndex, NUM_BOIDS);
}



void BoidsDrawer::preInitialise(std::shared_ptr<Entity> self, 
	Renderer* renderer, std::shared_ptr<GeometryLoader> loader) {
	GeometryLoader separateLoader;
	separateLoader.loadObjFile("fish.obj");
	std::unique_ptr<float[]> fishVertices = separateLoader.getVertices();
	int geometrySize = separateLoader.getNumVertexFloats();
	std::unique_ptr<unsigned int[]> fishIndices = separateLoader.getIndices();
	int indicesSize = separateLoader.getNumIndices();

	ModelRenderData mrd = separateLoader.getModelRenderData("fish.obj");
	firstIndexIndex = mrd.firstIndexIndex;
	numIndices = mrd.numIndices;
	texture = TextureFactory::getTextureFactory()->registerTexture("fish.jpg");
	generateGLObjects();
	boidShader.reset(new Shader("boid.vert", "boid.frag"));
	boidShader->useShader();
	glBindVertexArray(boidVAO);
	glBindBuffer(GL_ARRAY_BUFFER, fishVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fishEBO);

	glNamedBufferStorage(fishVBO,geometrySize * sizeof(float),fishVertices.get(), 0);
	glNamedBufferStorage(fishEBO,indicesSize * sizeof(unsigned int),fishIndices.get(), 0);
	
	//x y z
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//u v
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//nX nY nZ
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glEnable(GL_DEPTH_TEST);
}

void BoidsDrawer::initialise(std::shared_ptr<Entity> self, Renderer* renderer) {

}

void BoidsDrawer::generateGLObjects() {
	glGenVertexArrays(1, &boidVAO);
	GLuint buffers[2];
	glGenBuffers(2, buffers);
	fishVBO = buffers[0];
	fishEBO = buffers[1];
}

void BoidsDrawer::debugShaderStorageContent(void) {
	std::unique_ptr<float[]> boidData(new float[NUM_BOIDS*8]);
	glGetBufferSubData(GL_SHADER_STORAGE_BUFFER,0, NUM_BOIDS * 8 * sizeof(float), boidData.get());

	std::cout << "";
}

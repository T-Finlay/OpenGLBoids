#pragma once
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "Texture.h"
#include "Camera.h"
#include "Shader.h"
#include "Entity.h"
#include "EntityManager.h"
#include <memory>
#define NUM_BUFFERS 2
#define NUM_VAOS 2
class Renderer
{
public:
	GLuint Buffers[NUM_BUFFERS];
	GLuint VAOs[NUM_VAOS];
	void initialise(float*,int);
	void drawFrame(float);
	Renderer(int,int);
	int width, height;
	static void APIENTRY DebugCallBack(GLenum, GLenum, 
		GLuint, GLenum, GLsizei, const GLchar*, const void*);
	void drawEntity(Entity*);

private:
	std::shared_ptr<EntityManager> entityManager;
	std::unique_ptr<Shader> mainShader;
	std::unique_ptr<Shader> linesShader;
	std::unique_ptr<Camera> cam;
	glm::mat4 generateModelMatrix(Entity*);
	std::unique_ptr<Entity> testCube;
	void initTestCube();
	void drawAxisLines();
	void setupBuffers(float*, int);
	void drawAllEntities();
};


#pragma once
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Texture.h"
#include "Camera.h"
#include "Shader.h"
#include "Entity.h"
#include "EntityManager.h"
#include "GeometryLoader.h"
#include "EntityManager.h"
#include <memory>
#define NUM_BUFFERS 2
#define	NUM_EBOS 1
#define NUM_VAOS 2
class Renderer
{
public:
	GLuint Buffers[NUM_BUFFERS];
	GLuint VAOs[NUM_VAOS];
	GLuint EBOs[NUM_EBOS];
	void initialise(std::shared_ptr<EntityManager>);
	void drawFrame(float,std::shared_ptr<EntityManager>);
	Renderer(int,int,std::shared_ptr<GeometryLoader>);
	int width, height;
	static void APIENTRY DebugCallBack(GLenum, GLenum, 
		GLuint, GLenum, GLsizei, const GLchar*, const void*);
	void compileShaders();
	std::shared_ptr<Shader> getMainShader();
	GLuint getMainVao();
	ModelRenderData getModelData(std::string model);

private:
	std::shared_ptr<Shader> mainShader;
	std::shared_ptr<Shader> linesShader;
	std::shared_ptr<Camera> cam;
	std::unique_ptr<Entity> testCube;
	std::shared_ptr<GeometryLoader> loader;
	void createBuffers();
	void drawAxisLines();
	void setupBuffers(std::shared_ptr<EntityManager>);
	void drawAllEntities(std::shared_ptr<EntityManager> entityManager);
};


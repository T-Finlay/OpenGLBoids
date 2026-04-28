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
class Renderer
{
public:
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
	void bindMainVAO();

private:
	GLuint mainVAO;
	GLuint mainBuffer;
	GLuint mainEBO;
	GLuint linesVAO;
	GLuint linesBuffer;
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


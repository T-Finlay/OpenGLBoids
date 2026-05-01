#pragma once
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <string>
#include <memory>
#include "Shader.h"
#include "Camera.h"
//#define DEBUG_SKYBOX
class Skybox
{
public:
	Skybox();
	void draw(std::shared_ptr<Camera> cam);

private:
#ifdef DEBUG_SKYBOX
	const std::string cubemapTextures[6] = {
		"testCubemap/debug_rt.jpg",
		"testCubemap/debug_lf.jpg",
		"testCubemap/debug_up.jpg",
		"testCubemap/debug_dn.jpg",
		"testCubemap/debug_ft.jpg",
		"testCubemap/debug_bk.jpg"
	};
#else
	const std::string cubemapTextures[6] = {
		"underwaterCubemap/uw_rt.jpg",
		"underwaterCubemap/uw_lf.jpg",
		"underwaterCubemap/uw_up.jpg",
		"underwaterCubemap/uw_dn.jpg",
		"underwaterCubemap/uw_ft.jpg",
		"underwaterCubemap/uw_bk.jpg"
	};
#endif

	float boxGeometry[24] = {
		
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f
	};

	unsigned int boxIndices[36] = {
		1, 2, 6,
		6, 5, 1,
		0, 4, 7,
		7, 3, 0,
		4, 5, 6,
		6, 7, 4,
		0, 3, 2,
		2, 1, 0,
		0, 1, 5,
		5, 4, 0,
		3, 7, 6,
		6, 2, 3
	};
	
	GLuint skyboxVAO, skyboxEBO, skyboxVBO,cubemapTexture;
	std::unique_ptr<Shader> skyboxShader;

	void generateCubemapTexture();
};


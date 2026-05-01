#include "Skybox.h"
#include "stb_image.h"
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

Skybox::Skybox() {
	GLuint bfrs[2];
	glCreateBuffers(2, bfrs);
	skyboxVBO = bfrs[0];
	skyboxEBO = bfrs[1];
	glCreateVertexArrays(1, &skyboxVAO);

	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);

	//8 vertices in a cube, 3 floats per vertex
	glNamedBufferStorage(skyboxVBO,sizeof(boxGeometry), boxGeometry, 0);

	//6 faces on a cube, each requiring 2 triangles, each requiring 3 vertex indicies
	glNamedBufferStorage(skyboxEBO,sizeof(boxIndices), boxIndices, 0);

	//positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	skyboxShader.reset(new Shader("skybox.vert","skybox.frag"));

	generateCubemapTexture();
}

void Skybox::draw(std::shared_ptr<Camera> cam) {
	if (cam == nullptr) {
		std::cout << "attempting to draw skybox with null camera, returning " << std::endl;
		return;
	}
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	
	skyboxShader->useShader();
	glBindVertexArray(skyboxVAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP,cubemapTexture);
	glm::mat4 viewmatrix = glm::mat4(glm::mat3(cam->viewMatrix));// downgrade and upgrade removes affect of translations to the viewmatrix
	glUniformMatrix4fv(skyboxShader->uniformLocation("view"), 1, GL_FALSE, glm::value_ptr(viewmatrix));
	glUniformMatrix4fv(skyboxShader->uniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(cam->projectionMatrix));

	glDrawElements(GL_TRIANGLES, 36 , GL_UNSIGNED_INT, 0);
}

void Skybox::generateCubemapTexture() {
	glEnable(GL_TEXTURE_CUBE_MAP);

	glGenTextures(1, &cubemapTexture);

	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	for (int i = 0;i < 6;i++) {
		int w, h, chan;
		stbi_set_flip_vertically_on_load(false); //cubemaps dont flip
		unsigned char* pxls = stbi_load(("textures/" + cubemapTextures[i]).c_str(), &w, &h, &chan, 0);
		if (pxls) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, pxls);
		}
		delete pxls;
		std::cout << "cubemap texture created from " << cubemapTextures[i].c_str() << " with ID " << cubemapTexture << std::endl;
	}
}

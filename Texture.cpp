#include "Texture.h"
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <string>
#include <iostream>

Texture::Texture(char* name)
{
	std::string path = "textures/" + std::string(name);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	GLuint texObject;
	glGenTextures(1, &texObject);
	glBindTexture(GL_TEXTURE_2D, texObject);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	int w, h, chan;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* pxls = stbi_load(path.c_str(), &w, &h, &chan, 0);
	if (pxls) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, pxls);
	}

	glGenerateMipmap(GL_TEXTURE_2D);
	delete pxls;

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	std::cout << "texture created from " << name << " with ID " << texObject << std::endl;

	glTextureNumber = texObject;
}

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D,glTextureNumber);
}
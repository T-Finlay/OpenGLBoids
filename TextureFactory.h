#pragma once
#include <map>
#include <memory>
#include "Texture.h"
#include <string>
class TextureFactory
{
private:
	TextureFactory() {}
	std::map<std::string, std::shared_ptr<Texture>> textures;
	static TextureFactory* singleton;

public: 
	static TextureFactory* getTextureFactory();
	std::shared_ptr<Texture> registerTexture(std::string);
	static void cleanup();
};


#include "TextureFactory.h"

TextureFactory* TextureFactory::singleton = nullptr;

TextureFactory* TextureFactory::getTextureFactory() {
    if (singleton == nullptr) {
        singleton = new TextureFactory();
    }
    return singleton;
}

std::shared_ptr<Texture> TextureFactory::registerTexture(std::string textureName) {
    std::shared_ptr<Texture> tex = textures[textureName];
    if (tex != nullptr) {
        return tex;
    }
    tex = std::shared_ptr<Texture>(new Texture(textureName.c_str()));
    textures[textureName] = tex;
    return tex;
}

void TextureFactory::cleanup() {
    delete singleton;
}

#pragma once
class Texture
{
public:
	unsigned int glTextureNumber;
	void bind();

private:
	Texture(const char*);

	friend class TextureFactory;
};
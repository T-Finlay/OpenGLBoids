#pragma once
class Texture
{
public:
	unsigned int glTextureNumber;
	Texture(const char*);
	void bind();
};
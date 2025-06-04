#pragma once
class Texture
{
public:
	unsigned int glTextureNumber;
	Texture(char*);
	void bind();
};
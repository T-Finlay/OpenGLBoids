#pragma once
#include "texture.h"
class Entity {
public:
	unsigned int firstVertixIndex;
	unsigned int vertexCount;
	unsigned int VBOIndex;
	Texture texture;
};


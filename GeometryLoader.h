#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <map>
#define FLOATS_PER_VERTEX 8;
typedef struct VertexData {
	float x, y, z;
	float u, v;
	float nX, nY, nZ;

	std::string stringRepresentation();
} VertexData;

typedef struct ModelData {
	std::vector<VertexData> vertices;
	std::vector<unsigned int> indices;
} ModelData;

typedef struct TriData {
	unsigned int i1, i2, i3;
} TriData;

typedef struct ModelRenderData {
	unsigned int firstIndexIndex;
	unsigned int numIndices;
} ModelRenderData;

class GeometryLoader
{
private:
	std::unique_ptr<std::vector<VertexData>> loadOBJtoNonEBO(std::string);
	std::unique_ptr<ModelData> convertTriangulatedVerticesToModelData(std::unique_ptr<std::vector<VertexData>>);
	std::vector<float> rawVertexFloats;
	std::vector<unsigned int> indices;
	std::map<std::string, ModelRenderData> modelDataLookup;
	

public:
	unsigned int getNumVertexFloats(void);
	unsigned int getNumIndices(void);
	std::unique_ptr<float> getVertices(void);
	std::unique_ptr<unsigned int> getIndices(void);
	void loadObjFile(std::string);
	ModelRenderData getModelRenderData(std::string);
};


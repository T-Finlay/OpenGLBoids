#include "GeometryLoader.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include <memory>
#include <map>
#include <vector>
#include <xutility>


//slightly modified example code taken from tiny_obj_loader's github
std::unique_ptr<std::vector<VertexData>> GeometryLoader::loadOBJtoNonEBO(std::string inputfile) {
    std::unique_ptr<std::vector<VertexData>> triangulatedVertices(new std::vector<VertexData>);
    tinyobj::ObjReaderConfig reader_config;
    tinyobj::ObjReader reader;
    inputfile = "objs/" + inputfile;
    if (!reader.ParseFromFile(inputfile, reader_config)) {
        if (!reader.Error().empty()) {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        return nullptr;
    }

    if (!reader.Warning().empty()) {
        std::cout << "TinyObjReader: " << reader.Warning();
    }


    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                VertexData vtx;
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

                // access to vertex
                vtx.x = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                vtx.y = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                vtx.z = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

                // Check if `normal_index` is zero or positive. negative = no normal data
                if (idx.normal_index >= 0) {
                    vtx.nX = attrib.normals[3 * size_t(idx.normal_index) + 0];
                    vtx.nY = attrib.normals[3 * size_t(idx.normal_index) + 1];
                    vtx.nZ = attrib.normals[3 * size_t(idx.normal_index) + 2];
                }

                // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                if (idx.texcoord_index >= 0) {
                    vtx.u = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                    vtx.v = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                }

                triangulatedVertices->push_back(vtx);
            }
            index_offset += fv;
        }
    }
    return triangulatedVertices;
}

//take a list of triangulated vertices and de-duplicate it, returning a list of unique vertices and a list of triangulated indices of those vertices
std::unique_ptr<ModelData> GeometryLoader::convertTriangulatedVerticesToModelData(std::unique_ptr<std::vector<VertexData>> triangulatedVertices) {
    std::unique_ptr<ModelData> modelData(new ModelData);
    std::map<std::string, int> indexLookup;    
    //its totally a hacky work around, but because the default intial value for a map to ints is 0,
    //im going to use -1 to mean the zero index
    int currentIndex = -1;
    int testN = 0;
    size_t testSZ = sizeof(VertexData);
    for (auto vd : *triangulatedVertices) {
        testN++;

        std::string vertexLookupString = vd.stringRepresentation();
        int vertexIndex = indexLookup[vertexLookupString];

        //remember zero means no index not index = 0 because of our hacky workaround
        //so this vertex is new
        if (vertexIndex == 0) {
            modelData->vertices.push_back(vd);
            indexLookup[vertexLookupString] = currentIndex;

            //special case
            if (currentIndex == -1) {
                modelData->indices.push_back(0);
                currentIndex = 1;

            //general case
            } else {
                modelData->indices.push_back(currentIndex);
                currentIndex++;
            }
        
        //this vertex is not new
        } else {
            // so this is correcting for the case in which we have loaded a copy of vertex index 0 
            // which is saved as -1 because of our hacky workaround
            if (vertexIndex == -1) {
                modelData->indices.push_back(0);
            } else {
                modelData->indices.push_back(vertexIndex);
            }
        }
    }
    return modelData;
}

unsigned int GeometryLoader::getNumVertexFloats(void) {
    return rawVertexFloats.size();
}

unsigned int GeometryLoader::getNumIndices(void) {
    return indices.size();
}

std::unique_ptr<float> GeometryLoader::getVertices(void) {
    int numFloats = rawVertexFloats.size();
    std::unique_ptr<float> vertices(new float[numFloats]);
    std::copy(rawVertexFloats.begin(),rawVertexFloats.end(), &(*vertices));
    return vertices;
}

std::unique_ptr<unsigned int> GeometryLoader::getIndices(void) {
    int numInts = indices.size();
    std::unique_ptr<unsigned int> indicesarray(new unsigned int[numInts]);
    std::copy(indices.begin(), indices.end(), &(*indicesarray));
    return indicesarray;
}

void GeometryLoader::loadObjFile(std::string modelName) {
    std::unique_ptr<std::vector<VertexData>> vertices = loadOBJtoNonEBO(modelName);
    std::unique_ptr<ModelData> modelData = convertTriangulatedVerticesToModelData(std::move(vertices));
    ModelRenderData mrd;
    unsigned int initialNumVertices = (int) rawVertexFloats.size() / FLOATS_PER_VERTEX;
    mrd.firstIndexIndex = indices.size();
    
    for (const VertexData& vtx : modelData->vertices ) {
        rawVertexFloats.push_back(vtx.x);
        rawVertexFloats.push_back(vtx.y);
        rawVertexFloats.push_back(vtx.z);
        rawVertexFloats.push_back(vtx.u);
        rawVertexFloats.push_back(vtx.v);
        rawVertexFloats.push_back(vtx.nX);
        rawVertexFloats.push_back(vtx.nY);
        rawVertexFloats.push_back(vtx.nZ);
    }
    mrd.numIndices = 0;
    for (auto idx : modelData->indices) {
        indices.push_back(initialNumVertices + idx);
        mrd.numIndices++;
    }
    
    modelDataLookup[modelName] = mrd;
}

ModelRenderData GeometryLoader::getModelRenderData(std::string modelName) {
    return modelDataLookup[modelName];
}

std::string VertexData::stringRepresentation()
{
    std::string retval = "";
    for (int ptrAdd = 0; ptrAdd < sizeof(VertexData);ptrAdd++) {
        retval += *(((char*)this) + ptrAdd);
    }
    return retval;
}

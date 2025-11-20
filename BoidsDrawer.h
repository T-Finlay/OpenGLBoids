#pragma once
#include "DrawRoutine.h"

//forward declaration
class Texture;

class BoidsDrawer :
    public DrawRoutine
{
public:
    BoidsDrawer(GLuint* ptr);

    // Inherited via DrawRoutine
    void draw(std::shared_ptr<Entity> self, std::shared_ptr<Camera> cam, Renderer* renderer) override;
    void preInitialise(std::shared_ptr<Entity> self, Renderer* renderer, std::shared_ptr<GeometryLoader> loader) override;
    void initialise(std::shared_ptr<Entity> self, Renderer* renderer) override;

private:
    GLuint* boidDataBufferPointer;
    GLuint boidVAO,fishVBO,fishEBO;
    std::shared_ptr<Texture> texture;
    unsigned int firstIndexIndex;
    unsigned int numIndices;
    void generateGLObjects();
    std::unique_ptr<Shader> boidShader;
};


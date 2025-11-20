#pragma once
#include "Behaviour.h"
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "ComputeShader.h"
#include "glm/glm.hpp"

#define NUM_BOIDS 128
#define MAX_SINGLE_AXIS_DIST_FROM_ORIGIN 50.f

typedef struct BoidData {
    glm::vec4 pos = glm::vec4(1.f,1.f,1.f,1.f);
    glm::vec4 dir = glm::vec4(1.f, 1.f, 1.f, 1.f);;
} BoidData;

class BoidsManager :
    public Behaviour
{
public:
    BoidsManager(GLuint* ptr);
    // Inherited via Behaviour
    void initialise(std::shared_ptr<Entity> self) override;
    void update(float deltaTime, std::shared_ptr<Entity> self) override;

private:
    std::unique_ptr<ComputeShader> computeShader;
    GLuint readingBuffer, writingBuffer;
    void swapBuffers();
    std::unique_ptr<BoidData[]> initialiseBoidsData();
    GLuint* boidRenderBufferPointer;
};
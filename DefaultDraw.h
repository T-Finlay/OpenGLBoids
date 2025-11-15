#pragma once
#include "DrawRoutine.h"
#include "Texture.h"
#include <string>

class DefaultDraw :
    public DrawRoutine
{    
public:
    ~DefaultDraw();
    DefaultDraw(std::string model, std::string texture);
    void draw(std::shared_ptr<Entity> self, std::shared_ptr<Camera> cam) override;
    void initialise(std::shared_ptr<Entity> self, Renderer* renderer) override;

private:
    unsigned int firstIndexIndex;
    unsigned int indexCount;
    unsigned int VAOIndex;
    std::string textureName;
    std::string modelName;
    std::shared_ptr<Texture> texture;
};
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
    void draw(std::shared_ptr<Entity> self, std::shared_ptr<Camera> cam, Renderer* r) override;
    void preInitialise(std::shared_ptr<Entity> self, 
        Renderer* renderer,std::shared_ptr<GeometryLoader> loader) override;
    void initialise(std::shared_ptr<Entity> self, Renderer* renderer) override;

private:
    unsigned int firstIndexIndex;
    unsigned int indexCount;
    std::string textureName;
    std::string modelName;
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Texture> texture;
};
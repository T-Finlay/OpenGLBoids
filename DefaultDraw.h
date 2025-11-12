#pragma once
#include "DrawRoutine.h"

class DefaultDraw :
    public DrawRoutine
{    
public:
    ~DefaultDraw();
    void draw(std::shared_ptr<Entity> self, std::shared_ptr<Camera> cam);
    static std::unique_ptr<DefaultDraw> generateDefaultDraw();
    static void setDefaultShader(std::shared_ptr<Shader>);

private:
    static std::shared_ptr<Shader> defaultShader;
    DefaultDraw(std::shared_ptr<Shader>);
};
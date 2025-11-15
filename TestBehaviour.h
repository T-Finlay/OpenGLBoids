#pragma once
#include "Behaviour.h"
class TestBehaviour :
    public Behaviour
{
public:
    void initialise(std::shared_ptr<Entity> self) override {}
    void update(float deltaTime,std::shared_ptr<Entity> self) override;
};
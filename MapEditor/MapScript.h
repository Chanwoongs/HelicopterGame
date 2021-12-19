#pragma once
#include "Behaviour.h"
#include "Renderer.h"
#include "Transform.h"

class MapScript :
    public Behaviour
{
private:
    bool* map;

public:
    MapScript(GameObject* gameObject) : Behaviour(gameObject)
    {
        map = new bool[renderer->getCapacity()];
    }
    
    void update() override
    {

    }

};


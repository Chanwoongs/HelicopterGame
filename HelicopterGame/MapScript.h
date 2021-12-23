#pragma once
#include "Behaviour.h"
class MapScript :
    public Behaviour
{
private:
public:
    MapScript(GameObject* gameObject) : Behaviour(gameObject)
    {}
};


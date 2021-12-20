#pragma once
#include "Behaviour.h"
#include "Transform.h"
class BulletScript :
    public Behaviour
{
private:

public:
    BulletScript(GameObject* gameObject) : Behaviour(gameObject)
    {}

    void update() override
    {
        auto pos = transform->getPos();
        transform->setPos((pos + Position::right) + 2);
        
        //destroy(gameObject);
    }
};


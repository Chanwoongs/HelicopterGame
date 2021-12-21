#pragma once
#include "GameObject.h"
#include "Behaviour.h"
#include "Transform.h"
#include "Renderer.h"
class BulletScript :
    public Behaviour
{
private:
    void move()
    {
        auto pos = transform->getPos();
        transform->setPos((pos + Position::right) + 2);
    }
    void limitPos() 
    {
        if (transform->getPos().x + 1 > gameObject->getParent()->getRenderer()->getWidth())
        {
            destroy(gameObject);
        }
    }
public:
    BulletScript(GameObject* gameObject) : Behaviour(gameObject)
    {}

    void update() override
    {
        move();
        limitPos();
        //destroy(gameObject);
    }
};


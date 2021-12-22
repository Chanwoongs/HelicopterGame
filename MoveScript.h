#pragma once

#include "GameObject.h"
#include "Behaviour.h"
#include "Transform.h"
#include "Renderer.h"

class MoveScript :
    public Behaviour
{
private:
    GameObject* boundary;
public:
    MoveScript(GameObject* gameObject) : Behaviour(gameObject),
        boundary(gameObject->getParent())
    {

    }
    ~MoveScript() {}

    void update() override 
    {
        auto pos = transform->getPos();

        if (input->getKeyDown('W')) {
            if (transform->getPos().y - 1 < 0) return;
            transform->setPos(pos + Position::up);
        }
        if (input->getKeyDown('D')) {
            if (transform->getPos().x + renderer->getWidth() + 1 > boundary->getRenderer()->getWidth()) return;
            transform->setPos(pos + Position::right);
        }
        if (input->getKeyDown('A')) {
            if (transform->getPos().x - 2 < 0) return;
            transform->setPos((pos + Position::left) - 1);
        }
        if (input->getKeyDown('S')) {
            if (transform->getPos().y + renderer->getHeight() + 1 > boundary->getRenderer()->getHeight()) return;
            transform->setPos(pos + Position::down);
        }
    }
};


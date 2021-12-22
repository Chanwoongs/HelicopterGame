#pragma once
#include "GameObject.h"
#include "Behaviour.h"
#include "Transform.h"
#include "Renderer.h"
class BulletScript :
    public Behaviour
{
private:
    int bulletSpeed;
    int direction;

    // 총알 움직임
    void move()
    {
        auto pos = transform->getPos();
        transform->setPos(pos + (Position::right + bulletSpeed) * direction);
    }
    // 총알 범위 제한
    void limitPos() 
    {
        if (transform->getPos().x - 1 < 0 || transform->getPos().x + 1 > gameObject->getParent()->getRenderer()->getWidth())
        {
            destroy(gameObject);
        }
    }
public:
    BulletScript(GameObject* gameObject) : Behaviour(gameObject),
        bulletSpeed(1), direction(1)
    {}
    ~BulletScript() {}

    void update() override
    {
        move();
        limitPos();
    }

    void destroyBullet()
    {
        destroy(gameObject);
    }

    // 총알의 방향 세팅
    void setBulletDirection(int direction)
    {
        this->direction = direction;
    }
};


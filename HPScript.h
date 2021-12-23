#pragma once

#include "Behaviour.h"
#include "Renderer.h"

class HPScript :
    public Behaviour
{
private:
    string text;
    int hp;
public:
    HPScript(GameObject* gameObject) : Behaviour(gameObject),
        text(""), hp(0)
    { }
    ~HPScript() {}

    void start() override
    {
        for (int i = 0; i < renderer->getWidth(); i++)
        {
            text.append("\xDF");
        }
        renderer->setShape(text.c_str());
    }

    void update() override
    {
        hp = text.size() - 1;
        renderer->setShape(text.c_str());
    }

    int getHP() const
    {
        return hp;
    }
    // hp 초기화
    void resetHP()
    {
        for (int i = 0; i < renderer->getWidth(); i++)
        {
            text.append("\xDF");
        }
        hp = text.size() - 1;
    }
    // hp 감소
    void hit()
    {
        if (text.size() == 0) return;

        text = text.erase(text.size() - 1, 1);
    }
};


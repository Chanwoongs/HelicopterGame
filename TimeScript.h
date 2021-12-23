#pragma once
#include "Behaviour.h"
class TimeScript :
    public Behaviour
{
private:
    string text;
    string timeTxt;

    int time;
public:
    TimeScript(GameObject* gameObject) : Behaviour(gameObject),
        text(""), timeTxt(""), time(0)
    { }
    ~TimeScript() {}

    void update() override
    {
        printTime();
    }

    void setTime(int time)
    {
        this->time = time;
    }

    void printTime()
    {
        timeTxt = to_string(time);
        text = "    Time :     " + timeTxt + "s";
        renderer->setShape(text.c_str());
    }
};


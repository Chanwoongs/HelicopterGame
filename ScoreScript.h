#pragma once
#include "Behaviour.h"
class ScoreScript :
    public Behaviour
{
private:
    string text;
    string time;
public:
    ScoreScript(GameObject* gameObject) : Behaviour(gameObject)
    {

    }
    ~ScoreScript() {}


};


#pragma once

#include <cmath>
#include "Behaviour.h"
#include "Renderer.h"
#include "Transform.h"


class VerticalLineScript :
    public Behaviour
{
private:
    int pivot;
    int space;
    int nextDifference;
    int lineSize;
    char* shape;
    char sand;
    char grass;
    char ground;

public:
    VerticalLineScript(GameObject* gameObject) : Behaviour(gameObject),
        pivot(0), space(4), nextDifference(0), lineSize(0), shape(nullptr), sand(' '), grass(' '), ground(' ')
    {
        lineSize = renderer->getCapacity();
        shape = new char[lineSize];
        
        sand = '\xB0';
        grass = '\xB1';
        ground = '\xB2';
    }

    void update() override
    {
        setShape();
        move();
        limitPivot();
        limitSpace();
    }

    void move()
    {
        auto pos = transform->getPos();
        transform->setPos(pos + Position::left);
    }

    void setPivot(int pivot)
    {
        this->pivot = pivot;
    }
    int getPivot() const
    {
        return pivot;
    }

    void setSpace(int space)
    {
        this->space = space;
    }
    int getSpace() const
    {
        return space;
    }

    void setShape()
    {
        for (int i = 0; i < lineSize; i++)
        {
            shape[i] = ' ';
        }

        for (int i = 0; i < ceil((pivot - space) / 3); i++)
        {
            shape[i] = ground;
        }
        for (int i = ceil((pivot - space) / 3); i < ceil(((pivot - space) / 3) * 2); i++)
        {
            shape[i] = grass;
        }
        for (int i = ceil(((pivot - space) / 3) * 2); i < pivot - space; i++)
        {
            shape[i] = sand;
        }

        for (int i = pivot + space + 1; i < pivot + space + 1 + ceil(((lineSize - (pivot + space)) / 3)); i++)
        {
            shape[i] = sand;
        }
        for (int i = pivot + space + 1 + ceil(((lineSize - (pivot + space)) / 3)); i < pivot + space + 1 + ceil(((lineSize - (pivot + space)) / 3)) * 2; i++)
        {
            shape[i] = grass;
        }
        for (int i = pivot + space + 1 + ceil(((lineSize - (pivot + space)) / 3)) * 2; i < lineSize; i++)
        {
            shape[i] = ground;
        }
        
        renderer->setShape(shape);
    }

    void refresh(int pivot)
    {
        this->pivot = pivot;
    }

    void limitPivot()
    {
        if (pivot - space < 1)
        {
            pivot++;
        }
        else if (pivot + space > lineSize-2)
        {
            pivot--;
        }
    }
    void limitSpace()
    {
        if (space < 4)
        {
            space++;
        }
        else if (space > 7)
        {
            space--;
        }
    }
    
};
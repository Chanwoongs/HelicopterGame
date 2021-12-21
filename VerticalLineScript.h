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
    char grass;
    char sand;
    char rock;

    // 횡 이동
    void move()
    {
        auto pos = transform->getPos();
        transform->setPos(pos + Position::left);
    }

    // 한 줄의 terrain 설정
    void setShape()
    {
        for (int i = 0; i < lineSize; i++)
        {
            shape[i] = ' ';
        }

        for (int i = 0; i <= ceil((pivot - space) / 3); i++)
        {
            shape[i] = rock;
        }
        for (int i = ceil((pivot - space) / 3) + 1; i <= ceil(((pivot - space) / 3) * 2); i++)
        {
            shape[i] = sand;
        }
        for (int i = ceil(((pivot - space) / 3) * 2) + 1; i < pivot - space; i++)
        {
            shape[i] = grass;
        }

        for (int i = pivot + space + 1; i < pivot + space + 1 + ceil(((lineSize - (pivot + space)) / 3)); i++)
        {
            shape[i] = grass;
        }
        for (int i = pivot + space + 1 + ceil(((lineSize - (pivot + space)) / 3)); i < pivot + space + 1 + ceil(((lineSize - (pivot + space)) / 3)) * 2; i++)
        {
            shape[i] = sand;
        }
        for (int i = pivot + space + 1 + ceil(((lineSize - (pivot + space)) / 3)) * 2; i < lineSize; i++)
        {
            shape[i] = rock;
        }

        renderer->setShape(shape);
    }

    // pivot 값 제한
    void limitPivot()
    {
        if (pivot - space < 1)
        {
            pivot++;
        }
        else if (pivot + space > lineSize - 2)
        {
            pivot--;
        }
    }
    // space 값 제한
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

public:
    VerticalLineScript(GameObject* gameObject) : Behaviour(gameObject),
        pivot(0), space(4), nextDifference(0), lineSize(0), shape(nullptr), grass(' '), sand(' '), rock(' ')
    {
        lineSize = renderer->getCapacity();
        shape = new char[lineSize];
        
        grass = '\xB0';
        sand = '\xB1';
        rock = '\xB2';
    }

    void update() override
    {
        setShape();
        move();
        limitPivot();
        limitSpace();
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

    // pivot 값 새로고침
    void refresh(int pivot)
    {
        this->pivot = pivot;
    }
};
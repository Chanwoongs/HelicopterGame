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

    Position topPos;
    Position bottomPos;

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
    // 위에 있는 지형의 가장 아래 인덱스
    void updateTopPos()
    {
        topPos = Position(transform->getPos().x, pivot - space - 1);
    }
    // 아래 있는 지형의 가장 위 인덱스
    void updateBottomPos()
    {
        bottomPos = Position(transform->getPos().x, pivot + space + 1);
    }

public:
    VerticalLineScript(GameObject* gameObject) : Behaviour(gameObject),
        pivot(0), space(4), nextDifference(0), lineSize(0), shape(nullptr), grass(' '), sand(' '), rock(' '), topPos(0,0), bottomPos(0,0)
    {
        lineSize = renderer->getCapacity();
        shape = new char[lineSize];
        
        grass = '\xB0';
        sand = '\xB1';
        rock = '\xB2';
    }
    ~VerticalLineScript() {}

    void update() override
    {
        setShape();
        move();
        limitPivot();
        limitSpace();
        updateTopPos();
        updateBottomPos();
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

    Position getTopPos()
    {
        return topPos;
    }
    Position getBottomPos()
    {
        return bottomPos;
    }

    // pivot 값 새로고침
    void refresh(int pivot)
    {
        this->pivot = pivot;
    }
};
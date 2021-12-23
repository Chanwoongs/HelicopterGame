#pragma once
#include "Behaviour.h"
#include "Transform.h"
#include <functional>

class Button :
    public Behaviour
{
private:
    string text;
    string name;

    int buttonSize;

public:
    function<void(void)> onClick;

    Button(GameObject* gameObject) : Behaviour(gameObject),
        text(""), name(""), buttonSize(0), onClick([]() {})
    {
        // 버튼 크기
        buttonSize = renderer->getCapacity();

        // 버튼 문자 초기화
        for (int i = 0; i < buttonSize; i++)
        {
            text.append(" ");
        }
    }

    void update() override
    {
        printButton();
    }

    // 버튼 내용 세팅
    void setName(string name)
    {
        this->name = name;
    }

    // 버튼 내용 출력
    void printButton()
    {
        // 버튼의 중간부터 내용의 길이만큼 출력 (내용의 길이에 따라 출력 시작위치 변경)
        text.replace((buttonSize / 2) - (name.length() / 2), name.length(), name);

        // 내용 출력
        for (int i = 0; i < buttonSize; i++)
        {
            renderer->setShape(text[i], i);
        }
    }
};


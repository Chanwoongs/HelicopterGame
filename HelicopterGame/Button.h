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
        // ��ư ũ��
        buttonSize = renderer->getCapacity();

        // ��ư ���� �ʱ�ȭ
        for (int i = 0; i < buttonSize; i++)
        {
            text.append(" ");
        }
    }

    void update() override
    {
        printButton();
    }

    // ��ư ���� ����
    void setName(string name)
    {
        this->name = name;
    }

    // ��ư ���� ���
    void printButton()
    {
        // ��ư�� �߰����� ������ ���̸�ŭ ��� (������ ���̿� ���� ��� ������ġ ����)
        text.replace((buttonSize / 2) - (name.length() / 2), name.length(), name);

        // ���� ���
        for (int i = 0; i < buttonSize; i++)
        {
            renderer->setShape(text[i], i);
        }
    }
};


#pragma once
#include "GameObject.h"
#include "Behaviour.h"
#include "Transform.h"
#include "BulletScript.h"

class HelicopterScript :
    public Behaviour
{
private:
    const char* image1;
    const char* image2;
    const char* image3;
    const char* image4;

    vector<const char*> images;
    int imageIndex;

    vector<Position> filledPoses;

    bool isFired;

    // ���� ���ư��� �ִϸ��̼�
    void rotateWing()
    {
        renderer->setShape(images.at(imageIndex));
        // ��� ��ȯ�� �� �ֵ��� index �� ����
        imageIndex = (imageIndex + 1) % images.size();
    }
    // �Ѿ� �߻� Ʈ����
    void fire()
    {
        if (input->getKeyDown(VK_SPACE))
        {
            isFired = true;
        }
    }
public:
    HelicopterScript(GameObject* gameObject) : Behaviour(gameObject),
        image1(nullptr), image2(nullptr), image3(nullptr), image4(nullptr), imageIndex(0), isFired(false)
    { }
    ~HelicopterScript() {}

    void start() override
    {
        image1 = "\xDB\xDB\xDB\xDB\xDB  \xDB  \xDB\xDB\xDB\xDB\xDB  \xDB\xDB\xDB";
        image2 = " \xDB\xDB\xDB   \xDB  \xDB\xDB\xDB\xDB\xDB  \xDB\xDB\xDB";
        image3 = "  \xDB    \xDB  \xDB\xDB\xDB\xDB\xDB  \xDB\xDB\xDB";
        image4 = " \xDB\xDB\xDB   \xDB  \xDB\xDB\xDB\xDB\xDB  \xDB\xDB\xDB";

        images.push_back(image1);
        images.push_back(image2);
        images.push_back(image3);
        images.push_back(image4);
    }

    void update() override 
    {
        rotateWing();
        fire();
    }

    bool getIsFired() const
    {
        return isFired;
    }
    void setIsFired(bool isFired)
    {
        this->isFired = isFired;
    }

    // �︮���� ���� ä�����ִ� �ε����� ����
    vector<Position> searchFilledPoses()
    {
        Position temp;

        for (int i = 0; i < renderer->getCapacity(); i++)
        {
            if (renderer->getShape()[i] != ' ')
            {
                temp = (Position(i % renderer->getWidth(), i / renderer->getWidth()) + transform->getPos());
                filledPoses.push_back(temp);
            }
        }

        return filledPoses;
    }
    // ���� �������� ä�����ִ� �ε������� �����ϱ� ���� Ŭ����
    void clearFilledPoses()
    {
        filledPoses.clear();
    }
};
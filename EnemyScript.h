#pragma once
#include "Behaviour.h"
#include "GameObject.h"
class EnemyScript :
    public Behaviour
{
private:
    const char* image1;
    const char* image2;
    const char* image3;
    const char* image4;
    const char* image5;

    vector<const char*> images;
    int imageIndex;
    int numOfImages;

    bool isFired;

    vector<Position> filledPoses;

    // �Ѿ��� �߻��ϴ� �ִϸ��̼�
    void enemyAnimation()
    {
        renderer->setShape(images.at(imageIndex));

        // �Ѿ� �߻� �Ŀ� ù �ִϸ��̼����� �ʱ�ȭ
        if (imageIndex == numOfImages - 1)
        {
            for (int i = 0; i < numOfImages - 1; i++)
            {
                images.pop_back();
            }
        }
        // �Ѿ��� �߻�Ǹ�
        if (isFired)
        {
            // �Ѿ� �߻� �ִϸ��̼� �ֱ�
            images.push_back(image2);
            images.push_back(image3);
            images.push_back(image4);
            images.push_back(image5);
             
            // �߻� �� �̹��� �� ����
            numOfImages = images.size();
            isFired = false;
        }
        // ��� ��ȯ�� �� �ֵ��� index �� ����
        imageIndex = (imageIndex + 1) % images.size();
        
    }
public:
    EnemyScript(GameObject* gameObject) : Behaviour(gameObject),
        image1(nullptr), image2(nullptr), image3(nullptr), image4(nullptr), image5(nullptr), imageIndex(0), numOfImages(0), isFired(false)
    {
        image1 = "  \xDB\xDB\xDB \xDB\xDB\xDB\xDB\xDB   \xDB\xDB\xDB \xB2\xB2\xB2\xB2\xB2\xB2";
        image2 = "  \xDB\xDB\xDB  \xDB\xDB\xDB\xDB   \xDB\xDB\xDB \xB2\xB2\xB2\xB2\xB2\xB2";
        image3 = "  \xDB\xDB\xDB   \xDB\xDB\xDB   \xDB\xDB\xDB \xB2\xB2\xB2\xB2\xB2\xB2";
        image4 = "  \xDB\xDB\xDB  \xDB\xDB\xDB\xDB   \xDB\xDB\xDB \xB2\xB2\xB2\xB2\xB2\xB2";
        image5 = "  \xDB\xDB\xDB \xDB\xDB\xDB\xDB\xDB   \xDB\xDB\xDB \xB2\xB2\xB2\xB2\xB2\xB2";

        images.push_back(image1);

        renderer->setShape(images[0]);
    }
    ~EnemyScript() {}

    void update() override
    {
        enemyAnimation();
    }

    void setIsFired(bool isFired)
    {
        this->isFired = isFired;
    }

    // �� ���� ä�����ִ� �ε����� ����
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


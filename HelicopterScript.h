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

    bool isFired;

    // 날개 돌아가는 애니메이션
    void rotateWing()
    {
        renderer->setShape(images.at(imageIndex));
        // 계속 순환할 수 있도록 index 값 변경
        imageIndex = (imageIndex + 1) % images.size();
    }
    // 총알 발사 트리거
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
    ~HelicopterScript() {}

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
};
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

    // 총알을 발사하는 애니메이션
    void enemyAnimation()
    {
        renderer->setShape(images.at(imageIndex));

        // 총알 발사 후에 첫 애니메이션으로 초기화
        if (imageIndex == numOfImages - 1)
        {
            for (int i = 0; i < numOfImages - 1; i++)
            {
                images.pop_back();
            }
        }
        // 총알이 발사되면
        if (isFired)
        {
            // 총알 발사 애니매이션 넣기
            images.push_back(image2);
            images.push_back(image3);
            images.push_back(image4);
            images.push_back(image5);
             
            // 발사 후 이미지 수 저장
            numOfImages = images.size();
            isFired = false;
        }
        // 계속 순환할 수 있도록 index 값 변경
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

    // 적 내에 채워져있는 인덱스들 저장
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
    // 다음 프레임의 채워져있는 인덱스들을 저장하기 위해 클리어
    void clearFilledPoses()
    {
        filledPoses.clear();
    }
};


#pragma once

#include <random>

#include "Position.h"
#include "GameObject.h"
#include "PanelRenderer.h"
#include "MoveScript.h"
#include "HelicopterScript.h"
#include "VerticalLineScript.h"

using namespace std;

class Scene : public GameObject
{
    bool    isCompleted;

    Input* input;

    random_device rd;
    int nextPivotDiff;
    int nextSpaceDiff;

    GameObject* helicopter;
    GameObject* boundary;

    vector<GameObject*> map;
    vector<GameObject*> bullets;

    /*vector<unique_ptr<GameObject>> bullets;*/
public:

    Scene()
        : GameObject{ nullptr, "root", "root", nullptr, {1, 1}, {0, 0}, Position::zeros }, isCompleted(false),
        input(Input::GetInstance()), nextPivotDiff(0), nextSpaceDiff(0), helicopter(nullptr), boundary(nullptr)
    {
        // 맵 경계선
        boundary = new GameObject(this, "boundary", "panel", nullptr, { 70, 20 }, { 5,5 }, Position::zeros);
        boundary->getOrAddComponent<PanelRenderer>();

        // 첫 번째 수직 선
        GameObject* firstVerticalLine = new GameObject(boundary, "verticalLine", "line", nullptr, { 1, boundary->getRenderer()->getHeight() }, { 0, 0 }, Position::zeros);
        // pivot 값 설정
        firstVerticalLine->getOrAddComponent<VerticalLineScript>()->setPivot(firstVerticalLine->getRenderer()->getHeight() / 2);
        // map vector container에 넣기 
        map.push_back(firstVerticalLine);
        // 이전 수직 선의 정보를 저장할 temp 생성
        GameObject* temp = firstVerticalLine;
        // map에 전부 넣기
        for (int i = 0; i < 74; i++)
        {
            // 다음 수직 선 생성
            GameObject* verticalLine = new GameObject(boundary, "verticalLine", "line", nullptr, { 1, boundary->getRenderer()->getHeight() }, { i + 1, 0 }, Position::zeros);
            // 이전 pivot 값을 받아와서 세팅
            verticalLine->getOrAddComponent<VerticalLineScript>()->setPivot(temp->getComponent<VerticalLineScript>()->getPivot());
            // map에 넣기
            map.push_back(verticalLine);
            // temp 에 현재 수직 선 저장 
            temp = verticalLine;
        }
        
        helicopter = new GameObject(boundary, "helicopter", "player", nullptr, { 5, 4 }, { 6, 8 }, Position::zeros);
        helicopter->addComponent<MoveScript>();
        helicopter->addComponent<HelicopterScript>();

       
    }

    void start() override { internalStart(); }

    void update() override;

    bool isSceneOver() const { return isCompleted; }

    void updateMap()
    {     
        // 난수 생성 -1, 0, 1
        mt19937 gen(rd());
        uniform_int_distribution<int> dis(-1, 1);
        // 다음 pivot 위치를 변경할 변수
        nextPivotDiff = dis(gen);
        // 다음 space 크기를 변경할 변수
        nextSpaceDiff = dis(gen);

        for (int i = 0; i < map.size(); i++)
        {
            // 수직 선이 경계선을 벗어나면
            if (map[i]->getTransform()->getPos().x < 0)
            {
                // 위치를 맨 끝으로 초기화
                map[i]->getTransform()->setPos(map.size() - 1, map[i]->getTransform()->getPos().y);
                // pivot 값에 nextPivotDiff를 더해 pivot 값 변경
                if (i == 0)
                {
                    map[i]->getComponent<VerticalLineScript>()->setSpace(map.back()->getComponent<VerticalLineScript>()->getSpace());
                    map[i]->getComponent<VerticalLineScript>()->refresh(map.back()->getComponent<VerticalLineScript>()->getPivot() + nextPivotDiff);
                }
                else
                {
                    map[i]->getComponent<VerticalLineScript>()->setSpace(map[i-1]->getComponent<VerticalLineScript>()->getSpace());
                    map[i]->getComponent<VerticalLineScript>()->refresh(map[i-1]->getComponent<VerticalLineScript>()->getPivot() + nextPivotDiff);
                }
                // pivot 값에 변화가 없다면 space 값에 nextSpaceDiff를 더해 space 크기 변경 (수직 선이 두칸 이상 차이나는 걸 방지하기 위해 pivot값에 변화가 없을 때만 실행)
                if (nextPivotDiff == 0)
                {
                    map[i]->getComponent<VerticalLineScript>()->setSpace(map[i]->getComponent<VerticalLineScript>()->getSpace() + nextSpaceDiff);
                }
            }
            // 경계선 범위 밖에 있는 수직선은 안보이게 처리
            if (map[i]->getTransform()->getPos().x > 69) 
            {
                map[i]->setHidden();
            }
            else
            {
                map[i]->setHidden(false);
            }
        }
     }

    bool checkCollision(GameObject* gameObject)
    {
        auto pos = gameObject->getTransform()->getPos();
        auto worldPos = gameObject->getTransform()->local2World(pos);
 
    }

#if 0
    void update() override {
        if (confirm && confirm->isActive() == false) {
            remove(confirm);
            delete confirm;
            confirm = nullptr;
        }
        if (input->getKey(VK_ESCAPE)) {
            auto dim = getDimension();
            confirm = new ConfirmationPanel{ getPos() + Position{ dim.x / 2 - 25/2, dim.y / 2 - 5/2}, 25, 5, this,
                [&]() { map->setFreeze(false); }, // if clicked cancel
                [&]() { isCompleted = true; } // if clicked okay
            };
            map->setFreeze(true);
        }
    }
#endif
};


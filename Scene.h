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
        // �� ��輱
        boundary = new GameObject(this, "boundary", "panel", nullptr, { 70, 20 }, { 5,5 }, Position::zeros);
        boundary->getOrAddComponent<PanelRenderer>();

        // ù ��° ���� ��
        GameObject* firstVerticalLine = new GameObject(boundary, "verticalLine", "line", nullptr, { 1, boundary->getRenderer()->getHeight() }, { 0, 0 }, Position::zeros);
        // pivot �� ����
        firstVerticalLine->getOrAddComponent<VerticalLineScript>()->setPivot(firstVerticalLine->getRenderer()->getHeight() / 2);
        // map vector container�� �ֱ� 
        map.push_back(firstVerticalLine);
        // ���� ���� ���� ������ ������ temp ����
        GameObject* temp = firstVerticalLine;
        // map�� ���� �ֱ�
        for (int i = 0; i < 74; i++)
        {
            // ���� ���� �� ����
            GameObject* verticalLine = new GameObject(boundary, "verticalLine", "line", nullptr, { 1, boundary->getRenderer()->getHeight() }, { i + 1, 0 }, Position::zeros);
            // ���� pivot ���� �޾ƿͼ� ����
            verticalLine->getOrAddComponent<VerticalLineScript>()->setPivot(temp->getComponent<VerticalLineScript>()->getPivot());
            // map�� �ֱ�
            map.push_back(verticalLine);
            // temp �� ���� ���� �� ���� 
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
        // ���� ���� -1, 0, 1
        mt19937 gen(rd());
        uniform_int_distribution<int> dis(-1, 1);
        // ���� pivot ��ġ�� ������ ����
        nextPivotDiff = dis(gen);
        // ���� space ũ�⸦ ������ ����
        nextSpaceDiff = dis(gen);

        for (int i = 0; i < map.size(); i++)
        {
            // ���� ���� ��輱�� �����
            if (map[i]->getTransform()->getPos().x < 0)
            {
                // ��ġ�� �� ������ �ʱ�ȭ
                map[i]->getTransform()->setPos(map.size() - 1, map[i]->getTransform()->getPos().y);
                // pivot ���� nextPivotDiff�� ���� pivot �� ����
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
                // pivot ���� ��ȭ�� ���ٸ� space ���� nextSpaceDiff�� ���� space ũ�� ���� (���� ���� ��ĭ �̻� ���̳��� �� �����ϱ� ���� pivot���� ��ȭ�� ���� ���� ����)
                if (nextPivotDiff == 0)
                {
                    map[i]->getComponent<VerticalLineScript>()->setSpace(map[i]->getComponent<VerticalLineScript>()->getSpace() + nextSpaceDiff);
                }
            }
            // ��輱 ���� �ۿ� �ִ� �������� �Ⱥ��̰� ó��
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


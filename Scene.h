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

        boundary = new GameObject(this, "boundary", "panel", nullptr, { 70, 20 }, { 5,5 }, Position::zeros);
        boundary->getOrAddComponent<PanelRenderer>();

        GameObject* currentVerticalLine = new GameObject(boundary, "verticalLine", "line", nullptr, { 1, 20 }, { 0, 0 }, Position::zeros);
        currentVerticalLine->getOrAddComponent<VerticalLineScript>()->setPivot(currentVerticalLine->getRenderer()->getHeight() / 2);
        map.push_back(currentVerticalLine);
        GameObject* temp = currentVerticalLine;
        for (int i = 0; i < 75; i++)
        {
            GameObject* verticalLine = new GameObject(boundary, "verticalLine", "line", nullptr, { 1, 20 }, { i + 1, 0 }, Position::zeros);
            verticalLine->getOrAddComponent<VerticalLineScript>()->setPivot(temp->getComponent<VerticalLineScript>()->getPivot());
            map.push_back(verticalLine);
            temp = verticalLine;
        }
        
        helicopter = new GameObject(this, "helicopter", "player", "", { 5, 4 }, { 6, 15 }, Position::zeros);
        helicopter->addComponent<MoveScript>();
        helicopter->addComponent<HelicopterScript>();

       
    }

    void start() override { internalStart(); }

    void update() override;

    bool isSceneOver() const { return isCompleted; }

    void updateMap()
    {        
        mt19937 gen(rd());
        uniform_int_distribution<int> dis(-1, 1);
        nextPivotDiff = dis(gen);
        nextSpaceDiff = dis(gen);

        for (int i = 0; i < map.size(); i++)
        {
            if (map[i]->getTransform()->getPos().x < 0)
            {
                map[i]->getTransform()->setPos(map[i]->getTransform()->getPos().x + map.size(), map[i]->getTransform()->getPos().y);
     
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
                if (nextPivotDiff == 0)
                {
                    map[i]->getComponent<VerticalLineScript>()->setSpace(map[i]->getComponent<VerticalLineScript>()->getSpace() + nextSpaceDiff);
                }
            }
            
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


#pragma once

#include "Position.h"
#include "GameObject.h"
#include "PanelRenderer.h"
#include "MoveScript.h"
#include "HelicopterScript.h"

using namespace std;

class Scene : public GameObject
{
    bool    isCompleted;

    Input*  input;

    GameObject* helicopter;
    vector<unique_ptr<GameObject>> bullets;

public:

    Scene()
        : GameObject{ nullptr, "root", "root", nullptr, {1, 1}, {0, 0}, Position::zeros }, isCompleted(false),
        input(Input::GetInstance()), helicopter(nullptr)
    {
        helicopter = new GameObject(this, "helicopter", "player", "", { 5, 4 }, { 5, 15 }, Position::zeros);
        helicopter->addComponent<MoveScript>();
        helicopter->addComponent<HelicopterScript>();
    }

    void start() override { internalStart(); }

    void update() override;

    bool isSceneOver() const { return isCompleted; }

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


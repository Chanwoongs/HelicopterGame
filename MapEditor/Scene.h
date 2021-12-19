#pragma once

#include "Position.h"
#include "GameObject.h"
#include "PanelRenderer.h"
#include "MapEditorScript.h"
#include "Button.h"

using namespace std;

class Scene : public GameObject
{
    bool    isCompleted;

    Input*  input;

    GameObject* map;
    GameObject* sandButton;
    GameObject* grassButton;
    GameObject* groundButton;
    GameObject* clearButton;
    GameObject* saveButton;
    GameObject* loadButton;

    MapEditorScript* mapEditor;
public:

    Scene() 
        : GameObject{ nullptr, "root", "root", nullptr, {1, 1}, {0, 0}, Position::zeros }, isCompleted(false),
        input( Input::GetInstance() )
    {
        // ������ �� �߰�
        map = new GameObject(this, "map", "panel", nullptr, { 70, 20 }, { 1,1 }, Position::zeros);
        map->getOrAddComponent<PanelRenderer>()->setTitle("Edit Map");
        map->addComponent<MapEditorScript>();
        mapEditor = map->getComponent<MapEditorScript>();

        // sand Terrain ���� ��ư
        sandButton = new GameObject(this, "sandButton", "button", nullptr, { 8, 3 }, { 1, 23 }, Position::zeros);
        sandButton->getOrAddComponent<PanelRenderer>()->setTitle("Sand");
        sandButton->getOrAddComponent<Button>()->setName("\xB0");
        sandButton->getOrAddComponent<Button>()->onClick = [&]() { mapEditor->setTerrain(1); };

        // grass Terrain ���� ��ư
        grassButton = new GameObject(this, "grassButton", "button", nullptr, { 8, 3 }, { 13, 23 }, Position::zeros);
        grassButton->getOrAddComponent<PanelRenderer>()->setTitle("Grass");
        grassButton->getOrAddComponent<Button>()->setName("\xB1");
        grassButton->getOrAddComponent<Button>()->onClick = [&]() { mapEditor->setTerrain(2); };

        // ground Terrain ���� ��ư
        groundButton = new GameObject(this, "groundButton", "button", nullptr, { 8, 3 }, { 25, 23 }, Position::zeros);
        groundButton->getOrAddComponent<PanelRenderer>()->setTitle("Ground");
        groundButton->getOrAddComponent<Button>()->setName("\xB2");
        groundButton->getOrAddComponent<Button>()->onClick = [&]() { mapEditor->setTerrain(3); };

        // �� �ʱ�ȭ ��ư
        clearButton = new GameObject(this, "clearButton", "button", nullptr, { 8, 3 }, { 39, 23 }, Position::zeros);
        clearButton->getOrAddComponent<PanelRenderer>();
        clearButton->getOrAddComponent<Button>()->setName("CLEAR");
        clearButton->getOrAddComponent<Button>()->onClick = [&]() { mapEditor->clear(); };

        // �� ���� ��ư
        saveButton = new GameObject(this, "saveButton", "button", nullptr, { 8, 3 }, { 51, 23 }, Position::zeros);
        saveButton->getOrAddComponent<PanelRenderer>();
        saveButton->getOrAddComponent<Button>()->setName("SAVE");
        saveButton->getOrAddComponent<Button>()->onClick = [&]() { mapEditor->save(); };

        // �� �ҷ����� ��ư
        loadButton = new GameObject(this, "loadButton", "button", nullptr, { 8, 3 }, { 63, 23 }, Position::zeros);
        loadButton->getOrAddComponent<PanelRenderer>();
        loadButton->getOrAddComponent<Button>()->setName("LOAD");
        loadButton->getOrAddComponent<Button>()->onClick = [&]() { mapEditor->load(); };
        
    }

    void start() override { internalStart(); }

    void update() override;

    bool isSceneOver() const { return isCompleted; }

    void customizeMap()
    {
        // ��Ŭ������ ���õ� Terrain ����
        Position mapPos = map->getTransform()->local2World(this->getTransform()->getPos());
        if (input->getMouseButtonDown(0) &&
            input->getMousePosition().x >= mapPos.x && input->getMousePosition().x < mapPos.x + map->getRenderer()->getWidth() &&
            input->getMousePosition().y >= mapPos.y && input->getMousePosition().y < mapPos.y + map->getRenderer()->getHeight())
        {
            mapEditor->editMap(input->getMousePosition());
        }
        // ��Ŭ������ ��ĭ���� ����
        if (input->getMouseButtonDown(1) &&
            input->getMousePosition().x >= mapPos.x && input->getMousePosition().x < mapPos.x + map->getRenderer()->getWidth() &&
            input->getMousePosition().y >= mapPos.y && input->getMousePosition().y < mapPos.y + map->getRenderer()->getHeight())
        {
            mapEditor->setBlank(input->getMousePosition());
        }
    }

    void onClickButton(GameObject* button)
    {
        // ��ư Ŭ�� �� onClick �Լ� ȣ��
        Position buttonPos = button->getTransform()->local2World(this->getTransform()->getPos());
        if (input->getMouseButtonDown(0) &&
            input->getMousePosition().x >= buttonPos.x && input->getMousePosition().x < buttonPos.x + button->getRenderer()->getWidth() &&
            input->getMousePosition().y >= buttonPos.y && input->getMousePosition().y < buttonPos.y + button->getRenderer()->getHeight())
        {
            button->getComponent<Button>()->onClick();
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


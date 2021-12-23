#pragma once

#include <random>

#include "Position.h"
#include "GameObject.h"
#include "PanelRenderer.h"
#include "MoveScript.h"
#include "HelicopterScript.h"
#include "VerticalLineScript.h"
#include "EnemyScript.h"
#include "TimeScript.h"
#include "HPScript.h"

using namespace std;

class Scene : public GameObject
{
    bool isCompleted;
    int enemySpawnTimer;
    int enemyFireTimer;

    Input* input;

    random_device rd;
    int nextPivotDiff;
    int nextSpaceDiff;

    GameObject* boundary;
    GameObject* helicopter;
    GameObject* enemy;
    GameObject* enemyHPBar;
    GameObject* timeBoard;
    GameObject* gameOverPanel;
    GameObject* gameOverTxt;

    vector<GameObject*> map;
    vector<GameObject*> bullets;

    float time;

public:
    Scene()
        : GameObject{ nullptr, "root", "root", nullptr, {1, 1}, {0, 0}, Position::zeros }, isCompleted(false),
        enemySpawnTimer(0), enemyFireTimer(0), input(Input::GetInstance()), nextPivotDiff(0), nextSpaceDiff(0),
        boundary(nullptr), helicopter(nullptr), enemy(nullptr), timeBoard(nullptr), time(0.f)
    {
        // �� ��輱
        boundary = new GameObject(this, "boundary", "panel", nullptr, { 70, 20 }, { 5,5 }, Position::zeros);
        boundary->addComponent<PanelRenderer>();

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

        enemy = new GameObject(boundary, "enemy", "enemy", nullptr, { 6, 4 }, { 0, 0 }, Position::zeros);
        enemy->getOrAddComponent<EnemyScript>();
        enemy->setHidden(true);

        enemyHPBar = new GameObject(enemy, "enemyHP", "UI", nullptr, { 6, 1 }, { 0, -1 }, Position::zeros);
        enemyHPBar->addComponent<HPScript>();

        timeBoard = new GameObject(this, "scoreBoard", "UI", "Score : ", { 20, 1 }, { 30, 27 }, Position::zeros);
        timeBoard->addComponent<PanelRenderer>();
        timeBoard->addComponent<TimeScript>();

        gameOverPanel = new GameObject(this, "gameOverPanel", "UI", nullptr, { 9, 1 }, { 35, 15 }, Position::zeros);
        gameOverPanel->addComponent<PanelRenderer>();
        gameOverTxt = new GameObject(gameOverPanel, "gameOverTxt", "UI", "GAME OVER", { 9, 1 }, { 0, 0 }, Position::zeros);
        gameOverPanel->setActive(false);

    }

    void start() override { internalStart(); }

    void update() override;

    bool isSceneOver() const { return isCompleted; }

    void firingHelicopter()
    {
        if (helicopter->getComponent<HelicopterScript>()->getIsFired())
        {
            GameObject* bullet = new GameObject(boundary, "bullet", "bullet", "-", { 1, 1 }, helicopter->getTransform()->getPos() + Position(5, 2), Position::zeros);
            bullet->getOrAddComponent<BulletScript>()->setBulletDirection(1);
            bullets.push_back(bullet);
            helicopter->getComponent<HelicopterScript>()->setIsFired(false);
        }
    }

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
                    map[i]->getComponent<VerticalLineScript>()->setSpace(map[i - 1]->getComponent<VerticalLineScript>()->getSpace());
                    map[i]->getComponent<VerticalLineScript>()->refresh(map[i - 1]->getComponent<VerticalLineScript>()->getPivot() + nextPivotDiff);
                }
                // pivot ���� ��ȭ�� ���ٸ� space ���� nextSpaceDiff�� ���� space ũ�� ���� (���� ���� ��ĭ �̻� ���̳��� �� �����ϱ� ���� pivot���� ��ȭ�� ���� ���� ����)
                if (nextPivotDiff == 0)
                {
                    map[i]->getComponent<VerticalLineScript>()->setSpace(map[i]->getComponent<VerticalLineScript>()->getSpace() + nextSpaceDiff);
                }
            }
            // ��輱 ���� �ۿ� �ִ� �������� �Ⱥ��̰� ó��
            if (map[i]->getTransform()->getPos().x > boundary->getRenderer()->getWidth() - 1)
            {
                map[i]->setHidden();
            }
            else
            {
                map[i]->setHidden(false);
            }
        }
    }

    // �浹 üũ
    void checkCollision()
    {
        // �︮���Ϳ� ä���� �ִ� �ε�����
        auto helicopterFilledPoses = helicopter->getComponent<HelicopterScript>()->searchFilledPoses();
        // ���� ä���� �ִ� �ε�����
        auto enemyFilledPoses = enemy->getComponent<EnemyScript>()->searchFilledPoses();

        // �︮���� �� �浹üũ
        for (int i = 0; i < helicopterFilledPoses.size(); i++)
        {
            for (int j = 0; j < map.size(); j++)
            {
                // �︮���Ϳ� ä���� ĭ�� ������ ��ġ�� �������ٸ�
                if (helicopterFilledPoses[i] == map[j]->getComponent<VerticalLineScript>()->getTopPos() ||
                    helicopterFilledPoses[i] == map[j]->getComponent<VerticalLineScript>()->getBottomPos())
                {
                    gameOverPanel->setActive(true);
                    isCompleted = true;
                }
            }
        }

        // �Ѿ� �浹üũ
        for (int i = 0; i < bullets.size(); i++)
        {
            // �Ѿ�, �� �浹üũ
            for (int j = 0; j < map.size(); j++)
            {
                // �Ѿ˰� ���� ���� �ݴ�� �����̰� �Ѿ��� speed�� �ֱ� ������ ���� ���� �ʵ��� �������� �浹 ó��
                if (bullets[i]->getTransform()->getPos() == map[j]->getComponent<VerticalLineScript>()->getTopPos() ||
                    bullets[i]->getTransform()->getPos() == map[j]->getComponent<VerticalLineScript>()->getBottomPos() ||
                    bullets[i]->getTransform()->getPos() - 1 == map[j]->getComponent<VerticalLineScript>()->getTopPos() ||
                    bullets[i]->getTransform()->getPos() - 1 == map[j]->getComponent<VerticalLineScript>()->getBottomPos() ||
                    bullets[i]->getTransform()->getPos() - 2 == map[j]->getComponent<VerticalLineScript>()->getTopPos() ||
                    bullets[i]->getTransform()->getPos() - 2 == map[j]->getComponent<VerticalLineScript>()->getBottomPos())
                {
                    // �Ѿ� ����
                    bullets[i]->getComponent<BulletScript>()->destroyBullet();
                }
            }
            // �Ѿ�, �︮���� �浹üũ
            for (int h = 0; h < helicopterFilledPoses.size(); h++)
            {
                if (bullets[i]->getTransform()->getPos() == helicopterFilledPoses[h])
                {
                    gameOverPanel->setActive(true);
                    isCompleted = true;
                }
            }
            // �Ѿ�, �� �浹üũ
            for (int k = 0; k < enemyFilledPoses.size(); k++)
            {
                if (bullets[i]->getTransform()->getPos() == enemyFilledPoses[k])
                {
                    // HP ����
                    enemyHPBar->getComponent<HPScript>()->hit();
                    
                    // HP�� 0�� �Ǹ�
                    if (enemyHPBar->getComponent<HPScript>()->getHP() == 0)
                    {
                        // HP ����
                        enemyHPBar->getComponent<HPScript>()->resetHP();
                        // �� ���ֱ�
                        enemy->setHidden();
                        // �� ���� �ð� �ʱ�ȭ
                        enemySpawnTimer = 0;
                    }
                    // �Ѿ� ����
                    bullets[i]->getComponent<BulletScript>()->destroyBullet();
                }
            }
        }
        // �ε����� �ʱ�ȭ
        helicopter->getComponent<HelicopterScript>()->clearFilledPoses();
        enemy->getComponent<EnemyScript>()->clearFilledPoses();
    }

    // Bullets Vector Container ����
    void updateBullets()
    {
        vector<GameObject*>::iterator it;

        for (int i = 0; i < bullets.size(); i++)
        {
            // bullet�� �׾��ٸ�
            if (!bullets[i]->isAlive())
            {
                // �ش� bullet�� bullets���� �����ֱ�
                it = bullets.begin() + i;
                bullets.erase(it);
            }
        }
    }

    // �� ��� 
    void updateEnemy()
    {
        // ���� �ð� ī��Ʈ 
        enemySpawnTimer++;
        // 20�����ӿ� 1�� �� ����
        if (enemySpawnTimer == 20)
        {
            enemy->setHidden(false);
        }
        // �̻��ÿ��� ���ܳ���
        if (enemy->isHidden())
        {
            enemy->getTransform()->setPos(0, 0);
        }
        else
        {
            updateEnemyPos();
        }
        firingEnemy();
    }

    // �� ��ġ ������Ʈ
    void updateEnemyPos()
    {
        for (int i = 0; i < map.size(); i++)
        {
            // pivot���� ���� ���� ���Ʒ� ������
            if (map[i]->getTransform()->getPos().x == (boundary->getRenderer()->getWidth() - enemy->getRenderer()->getWidth()))
            {
                enemy->getTransform()->setPos(boundary->getRenderer()->getWidth() - enemy->getRenderer()->getWidth(), map[i]->getComponent<VerticalLineScript>()->getPivot() - 2);
            }
        }
    }

    // �� �Ѿ� �ڵ� �߻�
    void firingEnemy()
    {
        if (enemy->isHidden()) return;
        // �߻� �ð� ī��Ʈ
        enemyFireTimer++;
        // 20�����ӿ� �� ���� �߻�
        if (enemyFireTimer == 20)
        {
            // �� �Ѿ� ����
            GameObject* bullet = new GameObject(boundary, "bullet", "bullet", "\xE0", { 1, 1 }, enemy->getTransform()->getPos() + Position(-1, 1), Position::zeros);
            bullet->getOrAddComponent<BulletScript>()->setBulletDirection(-1);
            bullets.push_back(bullet);

            // fire Ʈ���� Ȱ��ȭ
            enemy->getComponent<EnemyScript>()->setIsFired(true);

            // �߻� �ð� �ʱ�ȭ
            enemyFireTimer = 0;
        }
    }

    // �ð� ����
    void updateTime()
    {
        time++;
        timeBoard->getComponent<TimeScript>()->setTime(int(time) / 10);
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
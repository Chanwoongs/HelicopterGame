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
        // 맵 경계선
        boundary = new GameObject(this, "boundary", "panel", nullptr, { 70, 20 }, { 5,5 }, Position::zeros);
        boundary->addComponent<PanelRenderer>();

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
                    map[i]->getComponent<VerticalLineScript>()->setSpace(map[i - 1]->getComponent<VerticalLineScript>()->getSpace());
                    map[i]->getComponent<VerticalLineScript>()->refresh(map[i - 1]->getComponent<VerticalLineScript>()->getPivot() + nextPivotDiff);
                }
                // pivot 값에 변화가 없다면 space 값에 nextSpaceDiff를 더해 space 크기 변경 (수직 선이 두칸 이상 차이나는 걸 방지하기 위해 pivot값에 변화가 없을 때만 실행)
                if (nextPivotDiff == 0)
                {
                    map[i]->getComponent<VerticalLineScript>()->setSpace(map[i]->getComponent<VerticalLineScript>()->getSpace() + nextSpaceDiff);
                }
            }
            // 경계선 범위 밖에 있는 수직선은 안보이게 처리
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

    // 충돌 체크
    void checkCollision()
    {
        // 헬리콥터에 채워져 있는 인덱스들
        auto helicopterFilledPoses = helicopter->getComponent<HelicopterScript>()->searchFilledPoses();
        // 적에 채워져 있는 인덱스들
        auto enemyFilledPoses = enemy->getComponent<EnemyScript>()->searchFilledPoses();

        // 헬리콥터 맵 충돌체크
        for (int i = 0; i < helicopterFilledPoses.size(); i++)
        {
            for (int j = 0; j < map.size(); j++)
            {
                // 헬리콥터에 채워진 칸과 지형의 위치가 같아진다면
                if (helicopterFilledPoses[i] == map[j]->getComponent<VerticalLineScript>()->getTopPos() ||
                    helicopterFilledPoses[i] == map[j]->getComponent<VerticalLineScript>()->getBottomPos())
                {
                    gameOverPanel->setActive(true);
                    isCompleted = true;
                }
            }
        }

        // 총알 충돌체크
        for (int i = 0; i < bullets.size(); i++)
        {
            // 총알, 맵 충돌체크
            for (int j = 0; j < map.size(); j++)
            {
                // 총알과 맵은 서로 반대로 움직이고 총알은 speed가 있기 때문에 벽을 뚫지 않도록 다중으로 충돌 처리
                if (bullets[i]->getTransform()->getPos() == map[j]->getComponent<VerticalLineScript>()->getTopPos() ||
                    bullets[i]->getTransform()->getPos() == map[j]->getComponent<VerticalLineScript>()->getBottomPos() ||
                    bullets[i]->getTransform()->getPos() - 1 == map[j]->getComponent<VerticalLineScript>()->getTopPos() ||
                    bullets[i]->getTransform()->getPos() - 1 == map[j]->getComponent<VerticalLineScript>()->getBottomPos() ||
                    bullets[i]->getTransform()->getPos() - 2 == map[j]->getComponent<VerticalLineScript>()->getTopPos() ||
                    bullets[i]->getTransform()->getPos() - 2 == map[j]->getComponent<VerticalLineScript>()->getBottomPos())
                {
                    // 총알 삭제
                    bullets[i]->getComponent<BulletScript>()->destroyBullet();
                }
            }
            // 총알, 헬리콥터 충돌체크
            for (int h = 0; h < helicopterFilledPoses.size(); h++)
            {
                if (bullets[i]->getTransform()->getPos() == helicopterFilledPoses[h])
                {
                    gameOverPanel->setActive(true);
                    isCompleted = true;
                }
            }
            // 총알, 적 충돌체크
            for (int k = 0; k < enemyFilledPoses.size(); k++)
            {
                if (bullets[i]->getTransform()->getPos() == enemyFilledPoses[k])
                {
                    // HP 감소
                    enemyHPBar->getComponent<HPScript>()->hit();
                    
                    // HP가 0이 되면
                    if (enemyHPBar->getComponent<HPScript>()->getHP() == 0)
                    {
                        // HP 리셋
                        enemyHPBar->getComponent<HPScript>()->resetHP();
                        // 적 없애기
                        enemy->setHidden();
                        // 적 스폰 시간 초기화
                        enemySpawnTimer = 0;
                    }
                    // 총알 삭제
                    bullets[i]->getComponent<BulletScript>()->destroyBullet();
                }
            }
        }
        // 인덱스들 초기화
        helicopter->getComponent<HelicopterScript>()->clearFilledPoses();
        enemy->getComponent<EnemyScript>()->clearFilledPoses();
    }

    // Bullets Vector Container 관리
    void updateBullets()
    {
        vector<GameObject*>::iterator it;

        for (int i = 0; i < bullets.size(); i++)
        {
            // bullet이 죽었다면
            if (!bullets[i]->isAlive())
            {
                // 해당 bullet을 bullets에서 지워주기
                it = bullets.begin() + i;
                bullets.erase(it);
            }
        }
    }

    // 적 헬기 
    void updateEnemy()
    {
        // 출현 시간 카운트 
        enemySpawnTimer++;
        // 20프레임에 1번 씩 출현
        if (enemySpawnTimer == 20)
        {
            enemy->setHidden(false);
        }
        // 미사용시에는 숨겨놓기
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

    // 적 위치 업데이트
    void updateEnemyPos()
    {
        for (int i = 0; i < map.size(); i++)
        {
            // pivot값에 따라 적이 위아래 움직임
            if (map[i]->getTransform()->getPos().x == (boundary->getRenderer()->getWidth() - enemy->getRenderer()->getWidth()))
            {
                enemy->getTransform()->setPos(boundary->getRenderer()->getWidth() - enemy->getRenderer()->getWidth(), map[i]->getComponent<VerticalLineScript>()->getPivot() - 2);
            }
        }
    }

    // 적 총알 자동 발사
    void firingEnemy()
    {
        if (enemy->isHidden()) return;
        // 발사 시간 카운트
        enemyFireTimer++;
        // 20프레임에 한 번씩 발사
        if (enemyFireTimer == 20)
        {
            // 적 총알 생성
            GameObject* bullet = new GameObject(boundary, "bullet", "bullet", "\xE0", { 1, 1 }, enemy->getTransform()->getPos() + Position(-1, 1), Position::zeros);
            bullet->getOrAddComponent<BulletScript>()->setBulletDirection(-1);
            bullets.push_back(bullet);

            // fire 트리거 활성화
            enemy->getComponent<EnemyScript>()->setIsFired(true);

            // 발사 시간 초기화
            enemyFireTimer = 0;
        }
    }

    // 시간 측정
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
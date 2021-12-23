#include "Scene.h"

void Scene::update() {    

    if (input->getKey(VK_ESCAPE)) {
        isCompleted = true;
        return;
    }

    updateTime();
    firingHelicopter();   
    updateMap();
    updateEnemy();
    updateBullets();
    checkCollision();

    internalRemove();
    internalRender(Position::zeros, false);
    internalUpdate();
}
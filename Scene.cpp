#include "Scene.h"

void Scene::update() {    

    if (input->getKey(VK_ESCAPE)) {
        isCompleted = true;
        return;
    }

    if (helicopter->getComponent<HelicopterScript>()->getIsFired())
    {
        GameObject* bullet = new GameObject(boundary, "bullet", "bullet", "-", { 1, 1 }, helicopter->getTransform()->getPos() + Position(5, 2), Position::zeros);
        bullet->addComponent<BulletScript>();
        bullets.push_back(bullet);
        helicopter->getComponent<HelicopterScript>()->setIsFired(false);

        /*unique_ptr<GameObject> bullet{ new GameObject(this, "bullet", "bullet", "-", { 1, 1 }, helicopter->getTransform()->getPos() + Position(5, 2), Position::zeros) };
        bullet->addComponent<BulletScript>();
        bullets.push_back(bullet);
        helicopter->getComponent<HelicopterScript>()->setIsFired(false);*/
    }
    
    updateMap();

    internalRemove();
    internalRender(Position::zeros, false);
    internalUpdate();
}
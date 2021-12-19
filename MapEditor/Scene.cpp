#include "Scene.h"

void Scene::update() {    

    if (input->getKey(VK_ESCAPE)) {
        isCompleted = true;
        return;
    }

    customizeMap();
    onClickButton(sandButton);
    onClickButton(grassButton);
    onClickButton(groundButton);

    internalRemove();
    internalRender(Position::zeros, false);
    internalUpdate();
}

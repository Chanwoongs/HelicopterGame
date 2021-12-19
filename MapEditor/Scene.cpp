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
    onClickButton(clearButton);
    onClickButton(saveButton);
    onClickButton(loadButton);

    internalRemove();
    internalRender(Position::zeros, false);
    internalUpdate();
}

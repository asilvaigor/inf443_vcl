//
// Created by Aloysio Galvão Lopes on 2020-06-02.
//

#include "scene/Scene.h"
#include "shaders/Shaders.h"
#include "objects/dome/Dome.h"

int main() {
    std::string windowTitle = "Dome Test";
    auto scene = Scene::getInstance(windowTitle);

    auto forest = std::static_pointer_cast<Object>(std::make_shared<Dome>(scene.getShaders()));
    scene.addObject(forest);

    scene.display();
    return 0;
}
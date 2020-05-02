//
// Created by igor on 22/04/2020.
//

#include "scene/Scene.h"
#include "shaders/Shaders.h"
#include "objects/box/Box.h"
#include "objects/terrain/Terrain.h"

int main() {
    auto scene = Scene::getInstance("Shadow Test");

    auto terrain = std::static_pointer_cast<Object>(std::make_shared<Terrain>(scene.getShaders()));
    scene.addObject(terrain);

    auto box = std::static_pointer_cast<Object>(std::make_shared<Box>(Box(scene.getShaders(), {-8, 0, 0})));
    scene.addObject(box);

    box = std::static_pointer_cast<Object>(std::make_shared<Box>(Box(scene.getShaders(), {8, 0, 0})));
    scene.addObject(box);

    box = std::static_pointer_cast<Object>(std::make_shared<Box>(Box(scene.getShaders(), {0, -8, 0})));
    scene.addObject(box);

    box = std::static_pointer_cast<Object>(std::make_shared<Box>(Box(scene.getShaders(), {0, 8, 0})));
    scene.addObject(box);

    scene.display();
    return 0;
}

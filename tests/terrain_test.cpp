//
// Created by Aloysio Galvão Lopes on 2020-05-08.
//

#include "scene/Scene.h"
#include "shaders/Shaders.h"
#include "objects/terrain/MountainTerrain.h"

int main() {
    auto scene = Scene::getInstance("Terrain Test");

    auto terrain = std::static_pointer_cast<Object>(std::make_shared<MountainTerrain>(scene.getShaders(), 100, 100));
    scene.addObject(terrain);

    scene.display();
    return 0;
}

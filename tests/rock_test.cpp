//
// Created by igor on 22/04/2020.
//

#include "scene/Scene.h"
#include "shaders/Shaders.h"
#include "objects/rock/Rock.h"
#include "objects/terrain/FlatSurface.h"

int main() {
    auto scene = Scene::getInstance("Tree Test");

    auto terrain = std::static_pointer_cast<Object>(std::make_shared<FlatSurface>(scene.getShaders()));
    scene.addObject(terrain);

    vcl::vec3 pos;
    std::shared_ptr<Object> rock;

    pos = {0, 0, 0};
    rock = std::static_pointer_cast<Object>(
            std::make_shared<Rock>(scene.getShaders(), pos, 0.5, vcl::vec3(1.0, 0.5, 1.0), true));
    scene.addObject(rock);

    scene.display();
    return 0;
}

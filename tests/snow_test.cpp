//
// Created by igor on 22/04/2020.
//

#include "scene/Scene.h"
#include "shaders/Shaders.h"
#include "objects/snow/Snow.h"
#include "objects/terrain/FlatSurface.h"

int main() {
    auto scene = Scene::getInstance("Snow Test");

    auto terrain = std::static_pointer_cast<Object>(std::make_shared<FlatSurface>(scene.getShaders()));
    scene.addObject(terrain);

    auto snow = std::static_pointer_cast<Object>(
            std::make_shared<Snow>(scene.getShaders(), 250));
    scene.addObject(snow);

    scene.display();
    return 0;
}

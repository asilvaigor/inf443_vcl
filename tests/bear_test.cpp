//
// Created by igor on 13/05/2020.
//

#include "scene/Scene.h"
#include "shaders/Shaders.h"
#include "objects/bear/Bear.h"
#include "objects/terrain/MountainTerrain.h"
#include "objects/terrain/FlatSurface.h"

int main() {
    auto scene = Scene::getInstance("Bear Test");

    std::shared_ptr<BaseTerrain> terrain = std::make_shared<FlatSurface>(scene.getShaders());
    std::shared_ptr<Object> terrainObj = std::dynamic_pointer_cast<Object>(terrain);
    scene.addObject(terrainObj);

    vcl::vec3 pos;
    std::shared_ptr<Object> bear;

    pos = {0, 0, 0};
    bear = std::static_pointer_cast<Object>(
            std::make_shared<Bear>(scene.getShaders(), terrain, pos));
    scene.addObject(bear);

    scene.display();
    return 0;
}

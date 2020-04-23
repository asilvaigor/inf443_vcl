//
// Created by igor on 22/04/2020.
//

#include "scene/Scene.h"
#include "shaders/Shaders.h"
#include "objects/tree/Tree.h"

int main() {
    auto scene = Scene::getInstance("Tree Test");
    TreeSpecies species = TreeSpecies::pine();
    vcl::vec3 pos(0, 0, 0);
    auto tree = std::static_pointer_cast<Object>(std::make_shared<Tree>(scene.getShaders(), pos, species));
    scene.addObject(tree);
    scene.display();
    return 0;
}

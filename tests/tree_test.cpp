//
// Created by igor on 22/04/2020.
//

#include "scene/Scene.h"
#include "shaders/Shaders.h"
#include "objects/tree/Tree.h"
#include "objects/terrain/FlatSurface.h"

int main() {
    auto scene = Scene::getInstance("Tree Test");

    auto terrain = std::static_pointer_cast<Object>(std::make_shared<FlatSurface>(scene.getShaders()));
    scene.addObject(terrain);

    vcl::vec3 pos;
    std::shared_ptr<Object> tree;

    TreeSpecies pineSpecies = TreeSpecies::pine();
    pos = {0, 0, 0};
    tree = std::static_pointer_cast<Object>(
            std::make_shared<Tree>(scene.getShaders(), pos, pineSpecies, 0.4, true));
    scene.addObject(tree);

    TreeSpecies blackSpecies = TreeSpecies::blackTupelo();
    pos = {0, 5, 0};
    tree = std::static_pointer_cast<Object>(
            std::make_shared<Tree>(scene.getShaders(), pos, blackSpecies, 0.4, true));
    scene.addObject(tree);

    TreeSpecies bush1 = TreeSpecies::bush1();
    pos = {0, -3, 0};
    tree = std::static_pointer_cast<Object>(
            std::make_shared<Tree>(scene.getShaders(), pos, bush1, 0.4, true));
    scene.addObject(tree);

    TreeSpecies bush2 = TreeSpecies::bush2();
    pos = {0, 3, 0};
    tree = std::static_pointer_cast<Object>(
            std::make_shared<Tree>(scene.getShaders(), pos, bush2, 0.4, true));
    scene.addObject(tree);

    scene.display();
    return 0;
}

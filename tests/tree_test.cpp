//
// Created by igor on 22/04/2020.
//

#include "scene/Scene.h"
#include "shaders/Shaders.h"
#include "objects/tree/Tree.h"
#include "objects/terrain/Terrain.h"

int main() {
    auto scene = Scene::getInstance("Tree Test");

    auto terrain = std::static_pointer_cast<Object>(std::make_shared<Terrain>(scene.getShaders()));
    scene.addObject(terrain);

    TreeSpecies pineSpecies = TreeSpecies::pine();
    vcl::vec3 pos(0, 0, 0);
    auto tree = std::static_pointer_cast<Object>(
            std::make_shared<Tree>(scene.getShaders(), pos, pineSpecies, 0.4, true));
    scene.addObject(tree);

    TreeSpecies blackSpecies = TreeSpecies::blackTupelo();
    pos = {0, 5, 0};
    tree = std::static_pointer_cast<Object>(
            std::make_shared<Tree>(scene.getShaders(), pos, blackSpecies, 0.4, true));
    scene.addObject(tree);

    TreeSpecies bush = TreeSpecies::bush();
    pos = {0, -5, 0};
    tree = std::static_pointer_cast<Object>(
            std::make_shared<Tree>(scene.getShaders(), pos, bush, 0.4, true));
    scene.addObject(tree);

    scene.display();
    return 0;
}

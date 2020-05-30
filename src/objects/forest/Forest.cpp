//
// Created by igor on 27/05/2020.
//

#include "Forest.h"
#include "objects/tree/TreeSpecies.h"
#include "scene/Scene.h"

Forest::Forest(Shaders &shaders, std::shared_ptr<MountainTerrain> &terrain, int nTrees, int nBushes, int nRocks) :
        Object(false), generator(Scene::deterministic) {
    auto pine = TreeSpecies::pine();
    auto tupelo = TreeSpecies::blackTupelo();
    auto bush1 = TreeSpecies::bush1();
    auto bush2 = TreeSpecies::bush2();
    float minX = -0.5f * terrain->getXSize();
    float maxX = 0.5f * terrain->getXSize();
    float minY = -0.5f * terrain->getXSize();
    float maxY = 0.5f * terrain->getXSize();

    for (int i = 0; i < nTrees; i++) {
        auto &species = generator.rand() > 0 ? pine : tupelo;
        vcl::vec3 pos(generator.rand(minX, maxX), generator.rand(minY, maxY), 0.0f);
        while (terrain->isInsideLake(pos.x, pos.y)) {
            pos.x = generator.rand(minX, maxX);
            pos.y = generator.rand(minY, maxY);
        }
        pos.z = terrain->getTerrainHeight(pos.x, pos.y);
        float snowCoverage = 0.6f + 0.4f * pos.z / terrain->getMaxTerrainHeight();
        objects.push_back(std::make_shared<Tree>(shaders, pos, species, snowCoverage));
    }

    for (int i = 0; i < nBushes; i++) {
        auto &species = generator.rand() > 0 ? bush1 : bush2;
        vcl::vec3 pos(generator.rand(minX, maxX), generator.rand(minY, maxY), 0.0f);
        while (terrain->isInsideLake(pos.x, pos.y)) {
            pos.x = generator.rand(minX, maxX);
            pos.y = generator.rand(minY, maxY);
        }
        pos.z = terrain->getTerrainHeight(pos.x, pos.y);
        float snowCoverage = 0.6f + 0.4f * pos.z / terrain->getMaxTerrainHeight();
        objects.push_back(std::make_shared<Tree>(shaders, pos, species, snowCoverage));
    }

    boundingSphere = BoundingSphere({0, 0, 0}, hypotf(terrain->getXSize(), terrain->getYSize()));
}

void Forest::drawMesh(vcl::camera_scene &camera) {
    throw std::invalid_argument("Forst drawMesh should not be called because of bounding spheres. "
                                "Draw each of the objects separately.");
}

std::vector<std::shared_ptr<Object>> &Forest::getObjects() {
    return objects;
}

//
// Created by igor on 27/05/2020.
//

#include "Forest.h"
#include "objects/tree/TreeSpecies.h"
#include "scene/Scene.h"

Forest::Forest(Shaders &shaders, std::shared_ptr<BaseTerrain> &terrain, int nTrees, int nBushes, int nRocks) :
        Object(false), terrain(terrain), generator(Scene::deterministic) {
    generateTiles();

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
        while (terrain->isObstructed(pos.x, pos.y)) {
            pos.x = generator.rand(minX, maxX);
            pos.y = generator.rand(minY, maxY);
        }
        pos.z = terrain->getTerrainHeight(pos.x, pos.y);
        float snowCoverage = std::max(0.5f, 0.5f + 0.5f * pos.z / terrain->getMaxTerrainHeight());
        objects.push_back(std::make_shared<Tree>(shaders, pos, species, snowCoverage));
    }

    for (int i = 0; i < nBushes; i++) {
        auto &species = generator.rand() > 0 ? bush1 : bush2;
        vcl::vec3 pos(generator.rand(minX, maxX), generator.rand(minY, maxY), 0.0f);
        while (terrain->isObstructed(pos.x, pos.y)) {
            pos.x = generator.rand(minX, maxX);
            pos.y = generator.rand(minY, maxY);
        }
        pos.z = terrain->getTerrainHeight(pos.x, pos.y);
        float snowCoverage = std::max(0.5f, 0.5f + 0.5f * pos.z / terrain->getMaxTerrainHeight());
        objects.push_back(std::make_shared<Tree>(shaders, pos, species, snowCoverage));
    }
}

void Forest::drawMesh(vcl::camera_scene &, float) {
    throw std::invalid_argument("Forest drawMesh should not be called because of bounding spheres. "
                                "Draw each of the objects separately.");
}

std::vector<std::shared_ptr<Object>> &Forest::getObjects() {
    return objects;
}

ForestTile &Forest::getTile(float x, float y) {
    return tiles[(int) (x / ForestTile::TILE_SIZE)][(int) (y / ForestTile::TILE_SIZE)];
}

void Forest::generateTiles() {
    auto densityImg = vcl::image_load_png("../src/assets/textures/forestdensity.png", vcl::image_color_type::grey);
    int nrows = densityImg.height;
    int ncols = densityImg.width;
    const int ts = ForestTile::TILE_SIZE;

    // Density image must be compatible with terrain size
    assert(((int) round(terrain->getYSize())) / nrows == ts);
    assert(((int) round(terrain->getXSize())) / ncols == ts);

    for (int i = 0; i < nrows; i++) {
        tiles.push_back({});
        for (int j = 0; j < ncols; j++)
            tiles[i].emplace_back(ForestTile(densityImg.data[i * ncols + j] / 255.0f, i * ts, j * ts));
    }
}

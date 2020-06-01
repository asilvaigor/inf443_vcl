//
// Created by igor on 27/05/2020.
//

#include "Forest.h"
#include "scene/Scene.h"

Forest::Forest(Shaders &shaders, std::shared_ptr<BaseTerrain> &terrain, int nTrees, int nBushes, int nRocks) :
        Object(false), terrain(terrain), generator(Scene::deterministic) {
    generateTiles();
    generateTreesAndRocks(shaders, nTrees, nRocks);

    float minX = -0.5f * terrain->getXSize();
    float maxX = 0.5f * terrain->getXSize();
    float minY = -0.5f * terrain->getXSize();
    float maxY = 0.5f * terrain->getXSize();

    auto bush1 = TreeSpecies::bush1();
    auto bush2 = TreeSpecies::bush2();

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
    return tiles[(int) (x / (float) ForestTile::TILE_SIZE)][(int) (y / (float) ForestTile::TILE_SIZE)];
}

void Forest::generateTiles() {
    auto densityImg = vcl::image_load_png("../src/assets/textures/forestdensity.png", vcl::image_color_type::grey);
    int nrows = (int) densityImg.height;
    int ncols = (int) densityImg.width;
    const int ts = ForestTile::TILE_SIZE;

    // Density image must be compatible with terrain size
    assert(((int) round(terrain->getYSize())) / nrows == ts);
    assert(((int) round(terrain->getXSize())) / ncols == ts);

    for (int i = 0; i < nrows; i++) {
        tiles.emplace_back(std::vector<ForestTile>());
        for (int j = 0; j < ncols; j++) {
            float x = -0.5f * terrain->getXSize() + (float) (i * ts);
            float y = -0.5f * terrain->getYSize() + (float) (j * ts);
            tiles[i].emplace_back(ForestTile((float) densityImg.data[j * nrows + i] / 255.0f, x, y));
        }
    }
}

void Forest::generateTreesAndRocks(Shaders &shaders, int nTrees, int nRocks) {
    auto pine = TreeSpecies::pine();
    auto tupelo = TreeSpecies::blackTupelo();
    const float pineToTupeloRatio = 0.6f;
    int curNTrees = 0;
    int curNRocks = 0;
    shuffleTileList();

    std::vector<std::pair<int, int>> next({{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}});
    for (auto &p : tileList) {
        int i = p.first;
        int j = p.second;
        auto &t = tiles[i][j];
        if (t.isObstructed || t.containsObject)
            continue;
        for (auto &n : next)
            if (i + n.first >= 0 && i + n.first < (int) tiles.size() &&
                j + n.second >= 0 && j + n.second < (int) tiles[0].size() &&
                tiles[i + n.first][j + n.second].containsObject)
                continue;

        if (curNTrees < nTrees && generator.rand(0, 1) < t.treeProb) {
            vcl::vec3 pos;
            pos.x = generator.rand(t.minX, t.maxX);
            pos.y = generator.rand(t.minY, t.maxY);
            pos.z = terrain->getTerrainHeight(pos.x, pos.y);
            float snowCoverage = std::max(0.5f, 0.5f + 0.5f * pos.z / terrain->getMaxTerrainHeight());
            if (generator.rand(0, 1) < pineToTupeloRatio)
                objects.push_back(std::make_shared<Tree>(shaders, pos, pine, snowCoverage));
            else objects.push_back(std::make_shared<Tree>(shaders, pos, tupelo, snowCoverage));
            curNTrees++;
            t.containsObject = true;
        } else if (curNRocks < nRocks && generator.rand(0, 1) < t.rockProb) {
            vcl::vec3 pos;
            pos.x = generator.rand(t.minX, t.maxX);
            pos.y = generator.rand(t.minY, t.maxY);
            pos.z = terrain->getTerrainHeight(pos.x, pos.y);
            float snowCoverage = std::max(0.5f, 0.5f + 0.5f * pos.z / terrain->getMaxTerrainHeight());
            objects.push_back(std::make_shared<Rock>(shaders, pos, snowCoverage));
            curNRocks++;
            t.containsObject = true;
        }

        if (curNTrees == nTrees && curNRocks == nRocks)
            break;
    }
}

void Forest::shuffleTileList() {
    if (tileList.empty()) {
        tileList.reserve(tiles.size() * tiles[0].size());
        for (int i = 0; i < (int) tiles.size(); i++)
            for (int j = 0; j < (int) tiles[0].size(); j++)
                tileList.emplace_back(std::make_pair(i, j));
    }
    std::shuffle(tileList.begin(), tileList.end(), generator.getState());
}

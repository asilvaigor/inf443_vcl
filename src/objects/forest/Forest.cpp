//
// Created by igor on 27/05/2020.
//

#include "Forest.h"
#include "scene/Scene.h"
#include "objects/tree/TranslatedTree.h"

Forest::Forest(Shaders &shaders, std::shared_ptr<BaseTerrain> &terrain, int nTrees, int nBushes, int nRocks) :
        Object(false), terrain(terrain), generator(Scene::deterministic) {
    std::cout << "Loading forest... " << std::flush;
    generateTiles();
    generateTreesAndRocks(shaders, nTrees, nRocks);
    generateBushes(shaders, nBushes);
    std::cout << "Finished" << std::endl;
}

void Forest::drawMesh(vcl::camera_scene &) {
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
    std::cout << "Loading trees and rocks... " << std::flush;
    auto pine = TreeSpecies::pine();
    auto tupelo = TreeSpecies::blackTupelo();
    const float pineToTupeloRatio = 0.6f;
    const int maxUniqueTrees = 50;
    const float minSnowCoverage = 0.4;
    const float maxSnowCoverage = 0.7;
    int curNTrees = 0;
    int curNRocks = 0;

    // Generating all the trees
    vcl::vec3 origin;
    for (int i = 0; i < maxUniqueTrees; i++) {
        if (generator.rand(0, 1) < pineToTupeloRatio)
            trees.push_back(std::make_shared<Tree>(
                    shaders, origin, pine, generator.rand(minSnowCoverage, maxSnowCoverage)));
        else trees.push_back(std::make_shared<Tree>(
                shaders, origin, tupelo, generator.rand(minSnowCoverage, maxSnowCoverage)));
    }

    shuffleTileList();

    std::vector<std::pair<int, int>> next({{-1, -1},
                                           {-1, 0},
                                           {-1, 1},
                                           {0,  -1},
                                           {0,  1},
                                           {1,  -1},
                                           {1,  0},
                                           {1,  1}});
    while (curNTrees < nTrees && curNRocks < nRocks) {
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
                objects.push_back(std::make_shared<TranslatedTree>(
                        trees[round(generator.rand(0, maxUniqueTrees - 1))], pos));
                curNTrees++;
                t.tree = objects.back();
                t.containsObject = true;
            } else if (curNRocks < nRocks && generator.rand(0, 1) < t.rockProb) {
                vcl::vec3 pos;
                pos.x = generator.rand(t.minX, t.maxX);
                pos.y = generator.rand(t.minY, t.maxY);
                pos.z = terrain->getTerrainHeight(pos.x, pos.y);
                float snowCoverage = generator.rand(minSnowCoverage, maxSnowCoverage);
                vcl::vec3 axis(generator.rand(0.7f, 1.2f), generator.rand(0.4f, 0.6f), generator.rand(0.7f, 1.2f));
                float zAngle = generator.rand(0, M_PI);
                objects.push_back(std::make_shared<Rock>(shaders, pos, snowCoverage, axis, zAngle));
                curNRocks++;
                t.rock = objects.back();
                t.containsObject = true;
            }

            if (curNTrees == nTrees && curNRocks == nRocks)
                break;
        }
    }
}

void Forest::generateBushes(Shaders &shaders, int nBushes) {
    std::cout << "Loading bushes... " << std::flush;
    auto bush1 = TreeSpecies::bush1();
    auto bush2 = TreeSpecies::bush2();
    const float bush1to2Ratio = 0.5f;
    const int maxUniqueBushes = 50;
    int curNBushes = 0;

    // Generating all the bushes
    vcl::vec3 origin;
    for (int i = 0; i < maxUniqueBushes; i++) {
        if (generator.rand(0, 1) < bush1to2Ratio)
            bushes.push_back(std::make_shared<Tree>(shaders, origin, bush1, generator.rand(0.4, 0.7)));
        else bushes.push_back(std::make_shared<Tree>(shaders, origin, bush2, generator.rand(0.4, 0.7)));
    }

    shuffleTileList();

    while (curNBushes < nBushes) {
        for (auto &p : tileList) {
            int i = p.first;
            int j = p.second;
            auto &t = tiles[i][j];
            if (t.isObstructed)
                continue;

            if (generator.rand(0, 1) < t.bushProb) {
                vcl::vec3 pos;
                pos.x = generator.rand(t.minX, t.maxX);
                pos.y = generator.rand(t.minY, t.maxY);
                if (t.containsObject) {
                    if (t.tree != nullptr) {
                        auto tree = std::static_pointer_cast<TranslatedTree>(t.tree);
                        while (hypot(pos.x - tree->getPosition().x, pos.y - tree->getPosition().y) <
                               tree->getTrunkRadius()) {
                            pos.x = generator.rand(t.minX, t.maxX);
                            pos.y = generator.rand(t.minY, t.maxY);
                        }
                    } else {
                        auto &bb = t.rock->getBoundingBox();
                        while (bb.minX < pos.x && pos.x < bb.maxX && bb.minY < pos.y && pos.y < bb.maxY) {
                            pos.x = generator.rand(t.minX, t.maxX);
                            pos.y = generator.rand(t.minY, t.maxY);
                        }
                    }
                }
                pos.z = terrain->getTerrainHeight(pos.x, pos.y);
                objects.push_back(std::make_shared<TranslatedTree>(
                        bushes[round(generator.rand(0, maxUniqueBushes - 1))], pos));
                curNBushes++;
                t.bushProb -= 1.0f;
            }

            if (curNBushes == nBushes)
                break;
        }
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

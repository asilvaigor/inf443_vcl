//
// Created by igor on 27/05/2020.
//

#ifndef PGM_FOREST_H
#define PGM_FOREST_H

#include "objects/Object.h"
#include "objects/terrain/MountainTerrain.h"
#include "objects/tree/Tree.h"
#include "objects/rock/Rock.h"
#include "ForestTile.h"

class Forest : public Object {
public:
    Forest(Shaders &shaders, std::shared_ptr<BaseTerrain> &terrain,
           int nTrees = 400, int nBushes = 800, int nRocks = 50);

    void drawMesh(vcl::camera_scene &camera) override;

    std::vector<std::shared_ptr<Object>> &getObjects();

    ForestTile &getTile(float x, float y);

private:
    std::shared_ptr<BaseTerrain> &terrain;
    std::vector<std::shared_ptr<Object>> objects;
    std::vector<std::shared_ptr<Tree>> trees;
    std::vector<std::shared_ptr<Tree>> bushes;
    std::vector<std::vector<ForestTile>> tiles;
    std::vector<std::pair<int, int>> tileList;
    vcl::rand_generator generator;

    void generateTiles();

    void generateTreesAndRocks(Shaders &shaders, int nTrees, int nRocks);

    void generateBushes(Shaders &shaders, int nBushes);

    void shuffleTileList();
};


#endif //PGM_FOREST_H

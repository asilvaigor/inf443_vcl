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
           int nTrees = 100, int nBushes = 200, int nRocks = 20);

    void drawMesh(vcl::camera_scene &camera, float time = 0.0f) override;

    std::vector<std::shared_ptr<Object>> &getObjects();

    ForestTile &getTile(float x, float y);

private:
    std::shared_ptr<BaseTerrain> &terrain;
    std::vector<std::shared_ptr<Object>> objects;
    std::vector<std::vector<ForestTile>> tiles;
    vcl::rand_generator generator;

    void generateTiles();
};


#endif //PGM_FOREST_H

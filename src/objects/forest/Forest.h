//
// Created by igor on 27/05/2020.
//

#ifndef PGM_FOREST_H
#define PGM_FOREST_H

#include "objects/Object.h"
#include "objects/terrain/MountainTerrain.h"
#include "objects/tree/Tree.h"
#include "objects/rock/Rock.h"

class Forest : public Object {
public:
    Forest(Shaders &shaders, std::shared_ptr<MountainTerrain> &terrain, int nTrees = 5, int nBushes = 0,
            int nRocks = 0);

    void drawMesh(vcl::camera_scene &camera) override;

    std::vector<std::shared_ptr<Object>> &getObjects();

public:
    std::vector<std::shared_ptr<Object>> objects;
    vcl::rand_generator generator;
};


#endif //PGM_FOREST_H

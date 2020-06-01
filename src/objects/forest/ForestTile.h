//
// Created by igor on 01/06/2020.
//

#ifndef PGM_FORESTTILE_H
#define PGM_FORESTTILE_H

#include <vector>
#include <memory>
#include "objects/Object.h"

class ForestTile {
public:
    ForestTile(float density, float startX, float startY);

    std::shared_ptr<Object> tree;
    std::shared_ptr<Object> rock;
    std::vector<std::shared_ptr<Object>> bushes;
    float minX, maxX, minY, maxY;
    float treeProb;
    float bushProb;
    float rockProb;
    bool isObstructed; // By river or high slope
    bool containsObject; // Tree or rock

    static const int TILE_SIZE;
};


#endif //PGM_FORESTTILE_H

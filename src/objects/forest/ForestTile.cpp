//
// Created by igor on 01/06/2020.
//

#include "ForestTile.h"

const int ForestTile::TILE_SIZE = 5;

ForestTile::ForestTile(float density, float startX, float startY) {
    treeProb = density;
    rockProb = density * 0.5f;
    bushProb = density * 2.0f;
    isObstructed = density < 0.1f;
    containsObject = false;

    tree = nullptr;
    rock = nullptr;

    minX = startX;
    maxX = startX + TILE_SIZE;
    minY = startY;
    maxY = startY + TILE_SIZE;
}
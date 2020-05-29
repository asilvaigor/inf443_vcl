//
// Created by Aloysio Galvão Lopes on 2020-05-27.
//

#ifndef PGM_TERRAINFEATURE_H
#define PGM_TERRAINFEATURE_H


#include <vcl/math/vec/vec2/vec2.hpp>

class TerrainFeature {
public:
    TerrainFeature(vcl::vec2 pos, float sig, float height, bool type);

    float evaluate(float x, float y);

    bool isLowLevel();

private:
    bool type;
    float sig;
    float height;
    vcl::vec2 pos;
};


#endif //PGM_TERRAINFEATURE_H

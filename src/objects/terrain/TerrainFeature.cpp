//
// Created by Aloysio Galvão Lopes on 2020-05-27.
//

#include <vcl/math/helper_functions/norm/norm.hpp>
#include "TerrainFeature.h"

TerrainFeature::TerrainFeature(vcl::vec2 pos, float sig, float height, bool type) : pos(pos), sig(sig), height(height),
type(type) {}

float TerrainFeature::evaluate(float x, float y) {
    const float d = vcl::norm(vcl::vec2(x,y)-pos)/sig;
    return height*std::exp(-d*d);
}

bool TerrainFeature::isLowLevel() {
    return type;
}



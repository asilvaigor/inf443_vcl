//
// Created by igor on 21/04/2020.
//

#ifndef PGM_LEAF_H
#define PGM_LEAF_H

#include "vcl.hpp"
#include "TreeSpecies.h"
#include "utils/TurtleGraphics.h"

class Leaf {
public:
    Leaf(TreeSpecies &species, TurtleGraphics &turtle, float treeScale);

    vcl::mesh toMesh();

private:
    TurtleGraphics turtle;
    float scale;
    std::vector<std::pair<float, float>> description;
    vcl::buffer<vcl::uint3> connectivity;
    vcl::buffer<vcl::vec2> texture_uv;
    vcl::vec4 color;
};


#endif //PGM_LEAF_H

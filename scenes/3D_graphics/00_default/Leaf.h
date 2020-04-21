//
// Created by igor on 21/04/2020.
//

#ifndef PGM_LEAF_H
#define PGM_LEAF_H

#include "vcl/vcl.hpp"
#include "TreeSpecies.h"
#include "TurtleGraphics.h"

class Leaf {
public:
    Leaf(TreeSpecies &species, TurtleGraphics &turtle, float treeScale);

    vcl::mesh toMesh();

private:
    TurtleGraphics turtle;
    float scale;
    float scaleX;
    std::vector<std::pair<float, float>> description;
    vcl::buffer<vcl::uint3> connectivity;
    vcl::vec4 color;
};


#endif //PGM_LEAF_H

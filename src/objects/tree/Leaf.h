//
// Created by igor on 21/04/2020.
//

#ifndef PGM_LEAF_H
#define PGM_LEAF_H

#include "vcl.hpp"
#include "TreeSpecies.h"
#include "utils/TurtleGraphics.h"
#include "utils/BoundingSphere.h"

/**
 * Leaf object to be rendered. It consists of a triangle with two equal sides.
 */
class Leaf {
public:
    /**
     * Generates a leaf given a turtle for its origin. The origin will be the center of the triangle's base.
     * @param species
     * @param turtle Origin of the leaf.
     * @param treeScale Scale of the tree, calculated inside the Branch class.
     */
    Leaf(TreeSpecies &species, TurtleGraphics &turtle, float treeScale);

    /**
     * Converts the leaf to a mesh.
     * @return
     */
    vcl::mesh toMesh();

private:
    TurtleGraphics turtle;
    float scale;
    std::vector<std::pair<float, float>> description;
    vcl::buffer<vcl::uint3> connectivity;
    vcl::buffer<vcl::vec2> texture_uv;
};


#endif //PGM_LEAF_H

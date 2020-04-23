//
// Created by igor on 21/04/2020.
//

#include "Leaf.h"
#include "vcl.hpp"

Leaf::Leaf(TreeSpecies &species, TurtleGraphics &turtle, float treeScale) {
    this->turtle = turtle;
    scale = treeScale;

    // Describing leaf triangle in terms of turtle movements
    float height = species.leafHeight + vcl::rand_interval(-1, 1) * species.leafHeightVar;
    float base = species.leafBase + vcl::rand_interval(-1, 1) * species.leafBaseVar;
    const float side = hypot(height, base / 2);
    const float alpha = atan2(height, base / 2);
    description = {{-M_PI / 2, base / 2}, {M_PI - alpha, side}, {2 * alpha, side}};
    connectivity = {{0, 1, 2}};
    texture_uv = {{0, 0}, {0.1f, 1}, {0.2, 0}};
}

vcl::mesh Leaf::toMesh() {
    vcl::mesh mesh;
    auto origin = turtle.getPosition();
    auto normal = vcl::cross(turtle.getRight(), turtle.getDirection());

    for (auto &d : description) {
        turtle.turnRight(d.first);
        turtle.move(d.second);

        auto delta = turtle.getPosition() - origin;
        delta *= scale;
        mesh.position.push_back(origin + delta);
        mesh.normal.push_back(normal);
    }

    mesh.connectivity = connectivity;
    mesh.texture_uv = texture_uv;
    return mesh;
}

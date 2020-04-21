//
// Created by igor on 21/04/2020.
//

#include "Leaf.h"
#include "vcl/vcl.hpp"

Leaf::Leaf(TreeSpecies &species, TurtleGraphics &turtle, float treeScale) {
    this->turtle = turtle;
    scale = treeScale;

    // Describing leaf in terms of turtle movements
    float height = species.leafHeight * vcl::rand_interval(0.9, 1.1);
    float base = species.leafBase * vcl::rand_interval(0.9, 1.1);
    const float side = hypot(height, base / 2);
    const float alpha = atan2(height, base / 2);
    description = {{-M_PI / 2, base / 2}, {M_PI - alpha, side}, {2 * alpha, side}};

    color = species.leafColor +
            vcl::vec4(vcl::rand_interval(-1, 1) * species.leafColorVar[0],
                      vcl::rand_interval(-1, 1) * species.leafColorVar[1],
                      vcl::rand_interval(-1, 1) * species.leafColorVar[2],
                      vcl::rand_interval(-1, 1) * species.leafColorVar[3]);
    connectivity = {{0, 1, 2}};
}

vcl::mesh Leaf::toMesh() {
    vcl::mesh mesh;
    auto origin = turtle.getPosition();
    auto normal = vcl::cross(turtle.getDirection(), turtle.getRight());

    for (auto &d : description) {
        turtle.turnRight(d.first);
        turtle.move(d.second);

        auto delta = turtle.getPosition() - origin;
        delta *= scale;
        mesh.position.push_back(origin + delta);
        mesh.color.push_back(color);
        mesh.normal.push_back(normal);
    }

    mesh.connectivity = connectivity;
    return mesh;
}

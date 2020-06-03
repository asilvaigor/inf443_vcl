//
// Created by igor on 25/04/2020.
//

#include "Grid.h"

Grid::Grid(Shaders &shaders) : Object(false) {
    auto mesh = vcl::mesh_primitive_grid_3d(21, 21, 11, {-10, -10, 0});
    grid = vcl::mesh_drawable(mesh);
    grid.shader = shaders["wireframe"];
}

void Grid::drawMesh(vcl::camera_scene &camera) {
    vcl::draw(grid, camera);
}
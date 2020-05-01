//
// Created by igor on 23/04/2020.
//

#include "Terrain.h"

Terrain::Terrain(Shaders &shaders) : Object(false), snowTexture("snow") {
    vcl::vec3 p00 = {-10, -10, 0};
    vcl::vec3 p10 = {10, -10, 0};
    vcl::vec3 p11 = {10, 10, 0};
    vcl::vec3 p01 = {-10, 10, 0};
    vcl::mesh mesh = vcl::mesh_primitive_quad(p00, p10, p11, p01);
    boundingBox = BoundingBox(-10, 10, -10, 10, 0, 0);
    terrain = vcl::mesh_drawable(mesh);
    terrain.shader = shaders["mesh"];
    terrain.uniform.color = {1.0, 1.0, 1.0};
    terrain.uniform.shading = {0.5, 0.6, 0.1, 32};
}

void Terrain::draw(const vcl::camera_scene &camera) {
    terrain.uniform.light = *light;
    snowTexture.bind();
    vcl::draw(terrain, camera);
}

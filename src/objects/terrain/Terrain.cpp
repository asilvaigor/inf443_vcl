//
// Created by igor on 23/04/2020.
//

#include "Terrain.h"

Terrain::Terrain(Shaders &shaders) : Object(false), snowTexture("snow_ground") {
    light2 = std::make_shared<vcl::light_source>();
    light3 = std::make_shared<vcl::light_source>();

    float size = 140.0;
    vcl::vec3 p00 = {-size, -size, 0};
    vcl::vec3 p10 = {size, -size, 0};
    vcl::vec3 p11 = {size, size, 0};
    vcl::vec3 p01 = {-size, size, 0};
    vcl::mesh mesh = vcl::mesh_primitive_quad(p00, p10, p11, p01);
    mesh.texture_uv = {{0, 0}, {size, 0}, {size, size}, {0, size}};

    boundingBox = BoundingBox(-size, size, -size, size, 0, 0);

    terrain = vcl::mesh_drawable(mesh);
    terrain.shader = shaders["terrain_mesh"];
    terrain.uniform.color = {1.0, 1.0, 1.0};
    terrain.uniform.shading = {0.5, 0.6, 0.1, 32};
}

void Terrain::draw(vcl::camera_scene &camera) {
    terrain.uniform.light = light;
    terrain.uniform.light2 = light2;
    terrain.uniform.light3 = light3;
    snowTexture.bind();
    vcl::draw(terrain, camera);
}

void Terrain::setLight2(std::shared_ptr<vcl::light_source> &light) {
    light2 = light;
}

void Terrain::setLight3(std::shared_ptr<vcl::light_source> &light) {
    light3 = light;
}

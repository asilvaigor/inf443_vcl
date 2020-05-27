//
// Created by igor on 23/04/2020.
//

#include "FlatSurface.h"

FlatSurface::FlatSurface(Shaders &shaders) : BaseTerrain(), snowTexture("snow_ground") {
    float size = 140.0;
    vcl::vec3 p00 = {-size, -size, 0};
    vcl::vec3 p10 = {size, -size, 0};
    vcl::vec3 p11 = {size, size, 0};
    vcl::vec3 p01 = {-size, size, 0};
    vcl::mesh mesh = vcl::mesh_primitive_quad(p00, p10, p11, p01);
    mesh.texture_uv = {{0, 0}, {size, 0}, {size, size}, {0, size}};

    boundingSphere = BoundingSphere(mesh);

    terrain = vcl::mesh_drawable(mesh);
    terrain.shader = shaders["terrain_mesh"];
    terrain.uniform.color = {1.0, 1.0, 1.0};
    terrain.uniform.shading = {0.5, 0.6, 0.1, 32};
}

void FlatSurface::drawMesh(vcl::camera_scene &camera, float) {
    terrain.uniform.lights = lights;
    terrain.uniform.current_light = currentLight;
    snowTexture.bind();
    vcl::draw(terrain, camera);
}

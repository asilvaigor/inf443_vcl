//
// Created by Aloysio Galvão Lopes on 2020-06-02.
//

#include "Dome.h"

Dome::Dome(Shaders &shaders) : Object(false, true), texture("skydome") {
    mesh = vcl::mesh_load_file_obj("../src/assets/models/skydome.obj");
    meshDrawable = vcl::mesh_drawable(mesh);
    meshDrawable.uniform.shading = {1.0f, 0.65f, 0.0, 1};
    meshDrawable.shader = shaders["mesh"];
}

void Dome::drawMesh(vcl::camera_scene &camera) {
    meshDrawable.uniform.lights = lights;
    texture.bind();
    vcl::draw(meshDrawable, camera);
}

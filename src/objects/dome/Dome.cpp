//
// Created by Aloysio Galvão Lopes on 2020-06-02.
//

#include "Dome.h"

Dome::Dome(Shaders &shaders) : Object(false){
    mesh = vcl::mesh_load_file_obj("../src/assets/models/skydome.obj");
    meshDrawable = vcl::mesh_drawable(mesh);
    meshDrawable.uniform.shading = shaders["mesh"];
}

void Dome::drawMesh(vcl::camera_scene &camera, float time) {
    meshDrawable.uniform.lights = lights;

    vcl::draw(meshDrawable, camera);
}

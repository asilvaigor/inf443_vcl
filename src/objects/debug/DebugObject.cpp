//
// Created by Aloysio Galvão Lopes on 2020-05-30.
//

#include "DebugObject.h"

DebugObject::DebugObject(Shaders &shaders) : Object(false), shaders(shaders) {}

void DebugObject::drawMesh(vcl::camera_scene &camera, float) {
    for (auto& mesh : meshes){
        mesh.uniform.lights = lights;
        vcl::draw(mesh, camera);
    }
}

void DebugObject::addMesh(vcl::mesh_drawable mesh) {
    mesh.shader = shaders["mesh"];
    meshes.emplace_back(mesh);
}

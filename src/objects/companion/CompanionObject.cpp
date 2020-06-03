//
// Created by Aloysio Galvão Lopes on 2020-06-02.
//

#include "CompanionObject.h"

CompanionObject::CompanionObject(Shaders &shaders, bool debug) : Object(true, true), debug(debug) {
    // Setting up animation timer
    currentTime = 0;

    // Setting up debug meshes
    if (debug){
        // TODO do not hardcode constants
        mesh = vcl::mesh_primitive_cone(2, {0, 0, 0}, {0, 0, 3});
        mesh.shader= shaders["mesh"];
    }
}

void CompanionObject::drawMesh(vcl::camera_scene &camera) {
    // Print meshes if in debug mode
    if (debug){
        // Direction and position
        mesh.uniform.lights = lights;
        mesh.uniform.transform.translation = position;
        mesh.uniform.transform.rotation = vcl::rotation_between_vector_mat3({0, 0, 1}, dp);
        vcl::draw(mesh, camera);
    }
}

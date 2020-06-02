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
        mesh.uniform.shading = shaders["mesh"];
        chargeMesh = vcl::mesh_primitive_sphere(1);
        chargeMesh.uniform.shading = shaders["mesh"];
    }
}

void CompanionObject::drawMesh(vcl::camera_scene &camera, float time) {
    // Animation
    if (time-currentTime > T)
        update(time);

    // Print meshes if in debug mode
    if (debug){
        // Direction and position
        mesh.uniform.transform.translation = position;
        mesh.uniform.transform.rotation = vcl::rotation_between_vector_mat3({0, 0, 1}, dp);
        vcl::draw(mesh, camera);

        // Charges
        chargeMesh.uniform.transform.translation = pc1;
        vcl::draw(chargeMesh, camera);
        chargeMesh.uniform.transform.translation = pc2;
        vcl::draw(chargeMesh, camera);
    }
}

void CompanionObject::updateChargesPositions() {
    vcl::vec3 dir = dp.normalized();
    pc1 = position+dir*dipoleOffset;
    pc2 = pc1+dir*distanceDipole;
}

vcl::vec2 CompanionObject::getFieldAt(vcl::vec2 pos) {
    throw std::invalid_argument("This method should be implemented in the subclass to be used");
}

vcl::vec3 CompanionObject::getFieldAt(vcl::vec3 pos) {
    throw std::invalid_argument("This method should be implemented in the subclass to be used");
}

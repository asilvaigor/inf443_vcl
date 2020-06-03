//
// Created by Aloysio Galvão Lopes on 2020-06-02.
//

#include "DipoleCompanion.h"

DipoleCompanion::DipoleCompanion(Shaders &shaders, bool debug) : CompanionObject(shaders, debug) {
    // Setting up debug meshes
    if (debug){
        // TODO do not hardcode constants
        chargeMesh = vcl::mesh_primitive_sphere(1);
        chargeMesh.shader = shaders["mesh"];
    }
}

void DipoleCompanion::drawMesh(vcl::camera_scene &camera) {
    // Draw for base object
    CompanionObject::drawMesh(camera);

    // Print meshes if in debug mode
    if (debug){
        // Charges
        chargeMesh.uniform.lights = lights;
        chargeMesh.uniform.transform.translation = pc1;
        vcl::draw(chargeMesh, camera);
        chargeMesh.uniform.transform.translation = pc2;
        vcl::draw(chargeMesh, camera);
    }
}

void DipoleCompanion::updateChargesPositions() {
    vcl::vec3 dir = dp.normalized();
    pc1 = position+dir*dipoleOffset;
    pc2 = pc1+dir*distanceDipole;
}

vcl::vec2 DipoleCompanion::getFieldAt(vcl::vec2 pos) {
    float k = charge;

    vcl::vec2 c1 = {pc1.x, pc1.y};
    vcl::vec2 c2 = {pc2.x, pc2.y};

    // For first charge
    float d1 = vcl::norm(pos-c1);
    vcl::vec2 e1 = -k*(pos-c1)/(d1*d1*d1);

    // For second change
    float d2 = vcl::norm(pos-c2);
    vcl::vec2 e2 = -k*(pos-c2)/(d2*d2*d2);

    return e1+e2;
}

vcl::vec3 DipoleCompanion::getFieldAt(vcl::vec3 pos) {
    float k = charge;

    // For first charge
    float d1 = vcl::norm(pos-pc1);
    vcl::vec3 e1 = -k*(pos-pc1)/(d1*d1*d1);

    // For second change
    float d2 = vcl::norm(pos-pc2);
    vcl::vec3 e2 = -k*(pos-pc2)/(d2*d2*d2);

    return e1+e2;
}

vcl::vec3 DipoleCompanion::getNegativeChargePosition() {
    return pc1;
}

void DipoleCompanion::update(float time) {
    updateChargesPositions();
}

#include <utility>

//
// Created by Aloysio Galvão Lopes on 2020-06-02.
//

#include "BearCompanion.h"

BearCompanion::BearCompanion(Shaders& shaders, vcl::CyclicCardinalSpline &trajectory,
        float initialS, std::shared_ptr<vcl::vec3> bearPosition) :
    Object(true, true), s(initialS), trajectory(trajectory), bearPosition(std::move(bearPosition)){
    dp = trajectory.position(s+ds)-trajectory.position(s-ds);
    position = trajectory.position(s);
    updateChargesPositions();
    currentTime = 0;

    mesh = vcl::mesh_primitive_cone(2, {0, 0, 0}, {0, 0, 3});
    mesh.uniform.shading = shaders["mesh"];
    chargeMesh = vcl::mesh_primitive_sphere(2);
    chargeMesh.uniform.shading = shaders["mesh"];
}

void BearCompanion::drawMesh(vcl::camera_scene &camera, float time) {
    positionUpdate(time);
    mesh.uniform.transform.translation = position;
    mesh.uniform.transform.rotation = vcl::rotation_between_vector_mat3({0, 0, 1}, dp);

    vcl::draw(mesh, camera);

    chargeMesh.uniform.transform.translation = pc1;
    vcl::draw(chargeMesh, camera);
    chargeMesh.uniform.transform.translation = pc2;
    vcl::draw(chargeMesh, camera);
}

void BearCompanion::positionUpdate(float time) {
    if (time-currentTime > T){
        currentTime = time;
        dp = trajectory.position(s+ds)-trajectory.position(s-ds);

        vcl::vec3 dist = *bearPosition-position;

        float movingForce = vcl::dot(dist.normalized(), dp.normalized());

        s += movingForce*frictionFactor;

        position = trajectory.position(s);
        updateChargesPositions();
    }
}

void BearCompanion::updateChargesPositions() {
    vcl::vec3 dir = dp.normalized();
    pc1 = position+dir*dipoleOffset;
    pc2 = pc1+dir*distanceDipole;
}

vcl::vec2 BearCompanion::getFieldAt(vcl::vec2 pos) {
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


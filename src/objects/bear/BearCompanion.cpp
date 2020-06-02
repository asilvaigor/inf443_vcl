#include <utility>

//
// Created by Aloysio Galvão Lopes on 2020-06-02.
//

#include "BearCompanion.h"

BearCompanion::BearCompanion(Shaders& shaders, vcl::CyclicCardinalSpline &trajectory,
        float initialS, std::shared_ptr<Bear>& bear) :
    Object(true, true), s(initialS), trajectory(trajectory), bear(bear){
    dp = trajectory.position(s+ds)-trajectory.position(s-ds);
    position = trajectory.position(s);
    currentTime = 0;

    mesh = vcl::mesh_primitive_cone(2, {0, 0, 0}, {0, 0, 3});
    mesh.uniform.shading = shaders["mesh"];
}

void BearCompanion::drawMesh(vcl::camera_scene &camera, float time) {
    positionUpdate(time);
    mesh.uniform.transform.translation = position;
    mesh.uniform.transform.rotation = vcl::rotation_between_vector_mat3({0, 0, 1}, dp);
    vcl::draw(mesh, camera);
}

void BearCompanion::positionUpdate(float time) {
    if (time-currentTime > T){
        currentTime = time;
        dp = trajectory.position(s+ds)-trajectory.position(s-ds);

        vcl::vec3 dist = bear->getPosition()-position;

        float movingForce = vcl::dot(dist.normalized(), dp.normalized());

//        std::cout << movingForce*frictionFactor << "\n";

        s += movingForce*frictionFactor;

        position = trajectory.position(s);
    }
}


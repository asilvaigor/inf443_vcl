#include <utility>

//
// Created by Aloysio Galvão Lopes on 2020-06-02.
//

#include "BearCompanion.h"

BearCompanion::BearCompanion(Shaders &shaders, vcl::CyclicCardinalSpline &trajectory, float initialS,
                             std::shared_ptr<vcl::vec3> bearPosition, bool debug) :
    CompanionObject(shaders, debug), s(initialS), trajectory(trajectory), bearPosition(std::move(bearPosition)){
    dp = trajectory.position(s+ds)-trajectory.position(s-ds);
    position = trajectory.position(s);
    updateChargesPositions();
}

void BearCompanion::update(float time) {
    currentTime = time;
    dp = trajectory.position(s+ds)-trajectory.position(s-ds);

    vcl::vec3 dist = *bearPosition-position;

    float movingForce = vcl::dot(dist.normalized(), dp.normalized());

    s += movingForce*frictionFactor;

    position = trajectory.position(s);
    updateChargesPositions();
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


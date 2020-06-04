#include <utility>

//
// Created by Aloysio Galvão Lopes on 2020-06-02.
//

#include "BearWalkingCompanion.h"

BearWalkingCompanion::BearWalkingCompanion(Shaders &shaders, std::shared_ptr<vcl::CyclicCardinalSpline> &trajectory, float initialS,
                             std::shared_ptr<vcl::vec3> bearPosition, bool debug) :
    CardinalSplineCompanion(shaders, std::static_pointer_cast<vcl::CardinalSpline>(trajectory), initialS, debug),
    bearPosition(std::move(bearPosition)) {}

void BearWalkingCompanion::update(float time) {
    dp = trajectory->position(s+ds)-trajectory->position(s-ds);

    vcl::vec3 dist = *bearPosition-position;

    float movingForce = vcl::dot(dist.normalized(), dp.normalized());

    s += movingForce*Constants::DIPOLE_FRICTION_FACTOR;

    position = trajectory->position(s);
    updateChargesPositions();
}

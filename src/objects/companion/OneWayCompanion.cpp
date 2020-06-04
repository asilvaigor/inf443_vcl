//
// Created by Aloysio Galvão Lopes on 2020-06-03.
//

#include "OneWayCompanion.h"

OneWayCompanion::OneWayCompanion(Shaders &shaders, std::shared_ptr<vcl::CardinalSpline> &trajectory, float initialS,
                                 bool active, bool debug) :
        ActivatableCompanion(shaders, trajectory, initialS, active, debug) {}

void OneWayCompanion::update(float time) {
    if (!active || s + ds > trajectory->getFinalTime())
        return;

    dp = trajectory->position(s + ds) - trajectory->position(s);

    s += movementRate;

    position = trajectory->position(s);
    updateChargesPositions();
}

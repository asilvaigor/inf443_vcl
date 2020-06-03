//
// Created by Aloysio Galvão Lopes on 2020-06-02.
//

#include "ActivatableCompanion.h"

ActivatableCompanion::ActivatableCompanion(Shaders &shaders, std::shared_ptr<vcl::CardinalSpline> trajectory,
                                           float initialS, bool active, bool debug) :
        CardinalSplineCompanion(shaders, trajectory, initialS, debug), active(active) {}

bool ActivatableCompanion::getActivationState() {
    return active;
}

void ActivatableCompanion::setActivationState(bool active) {
    this->active = active;
}

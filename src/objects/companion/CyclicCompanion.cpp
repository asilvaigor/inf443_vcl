//
// Created by Aloysio Galvão Lopes on 2020-06-02.
//

#include "CyclicCompanion.h"

CyclicCompanion::CyclicCompanion(Shaders &shaders, vcl::CyclicCardinalSpline &trajectory, float initialS, bool active,
        bool debug) : ActivatableCompanion(shaders, trajectory, initialS, active, debug){}

void CyclicCompanion::update(float time) {
    if (!active)
        return;
    // TODO remove this current time
    currentTime = time;
    
    dp = trajectory.position(s+ds)-trajectory.position(s-ds);

    s += movementRate;

    position = trajectory.position(s);
    updateChargesPositions();
}

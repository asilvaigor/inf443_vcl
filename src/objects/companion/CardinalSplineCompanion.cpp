//
// Created by Aloysio Galvão Lopes on 2020-06-02.
//

#include "CardinalSplineCompanion.h"

CardinalSplineCompanion::CardinalSplineCompanion(Shaders &shaders, std::shared_ptr<vcl::CardinalSpline> trajectory,
                                                 float initialS, bool debug) :
        DipoleCompanion(shaders, debug), s(initialS), trajectory(trajectory) {
    // Initializing position and orientation
    dp = trajectory->position(s + ds) - trajectory->position(s);
    position = trajectory->position(s);
    updateChargesPositions();
}
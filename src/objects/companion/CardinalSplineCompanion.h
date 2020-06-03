//
// Created by Aloysio Galvão Lopes on 2020-06-02.
//

#ifndef PGM_CARDINALSPLINECOMPANION_H
#define PGM_CARDINALSPLINECOMPANION_H

#include "CompanionObject.h"
#include "vcl.hpp"

class CardinalSplineCompanion : public CompanionObject {
public:
    CardinalSplineCompanion(Shaders &shaders, vcl::CardinalSpline &trajectory, float initialS, bool debug=true);

    vcl::vec2 getFieldAt(vcl::vec2 pos) override;

protected:
    // Trajectory
    float s;
    float ds = 0.01;
    vcl::CardinalSpline& trajectory;
};


#endif //PGM_CARDINALSPLINECOMPANION_H

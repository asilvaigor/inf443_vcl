//
// Created by Aloysio Galvão Lopes on 2020-06-02.
//

#ifndef PGM_CARDINALSPLINECOMPANION_H
#define PGM_CARDINALSPLINECOMPANION_H

#include "DipoleCompanion.h"
#include "vcl.hpp"

class CardinalSplineCompanion : public DipoleCompanion {
public:
    CardinalSplineCompanion(Shaders &shaders, std::shared_ptr<vcl::CardinalSpline> trajectory, float initialS,
            bool debug=true);
protected:
    // Trajectory
    float s;
    float ds = 0.01;
    std::shared_ptr<vcl::CardinalSpline> trajectory;
};


#endif //PGM_CARDINALSPLINECOMPANION_H

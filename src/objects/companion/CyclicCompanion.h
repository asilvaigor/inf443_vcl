//
// Created by Aloysio Galvão Lopes on 2020-06-02.
//

#ifndef PGM_CYCLICCOMPANION_H
#define PGM_CYCLICCOMPANION_H

#include "CardinalSplineCompanion.h"
#include "vcl.hpp"

class CyclicCompanion : public CardinalSplineCompanion {
public:
    CyclicCompanion(Shaders &shaders, vcl::CyclicCardinalSpline &trajectory, float initialS, bool debug=true);

    void update(float time) override;
private:
    const float movementRate = 0.005;
};


#endif //PGM_CYCLICCOMPANION_H

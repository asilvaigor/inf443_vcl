//
// Created by Aloysio Galvão Lopes on 2020-06-02.
//

#ifndef PGM_BEARWALKINGCOMPANION_H
#define PGM_BEARWALKINGCOMPANION_H

#include <vcl/math/spline/CyclicCardinalSpline.h>
#include "vcl.hpp"
#include "objects/companion/CardinalSplineCompanion.h"

class BearWalkingCompanion : public CardinalSplineCompanion {
public:
    BearWalkingCompanion(Shaders &shaders, std::shared_ptr<vcl::CyclicCardinalSpline> &trajectory, float initialS,
                  std::shared_ptr<vcl::vec3> bearPosition, bool debug = true);

    void update(float time) override;

private:
    // Bear
    std::shared_ptr<vcl::vec3> bearPosition;
};


#endif //PGM_BEARWALKINGCOMPANION_H

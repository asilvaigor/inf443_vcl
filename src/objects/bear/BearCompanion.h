//
// Created by Aloysio Galvão Lopes on 2020-06-02.
//

#ifndef PGM_BEARCOMPANION_H
#define PGM_BEARCOMPANION_H

#include <vcl/math/spline/CyclicCardinalSpline.h>
#include "vcl.hpp"
#include "objects/companion/CardinalSplineCompanion.h"

class BearCompanion : public CardinalSplineCompanion {
public:
    BearCompanion(Shaders &shaders, vcl::CyclicCardinalSpline &trajectory, float initialS,
                  std::shared_ptr<vcl::vec3> bearPosition, bool debug = true);

    void update(float time) override;

private:
    // Bear
    std::shared_ptr<vcl::vec3> bearPosition;
};


#endif //PGM_BEARCOMPANION_H

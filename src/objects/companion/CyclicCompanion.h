//
// Created by Aloysio Galvão Lopes on 2020-06-02.
//

#ifndef PGM_CYCLICCOMPANION_H
#define PGM_CYCLICCOMPANION_H

#include "ActivatableCompanion.h"
#include "vcl.hpp"

class CyclicCompanion : public ActivatableCompanion {
public:
    CyclicCompanion(Shaders &shaders, std::shared_ptr<vcl::CyclicCardinalSpline> &trajectory, float initialS,
            bool active = false, bool debug=true);

    void update(float time) override;
private:
    const float movementRate = 0.005;
};


#endif //PGM_CYCLICCOMPANION_H
